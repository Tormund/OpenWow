#pragma once

// Add data

#define __DBC_TVALUE(type, _name, _field) \
type Get_##_name() const \
{ \
	return getValue<type>(static_cast<uint32>(_field - 1)); \
}

#define __DBC_STRING(_name, _field) \
const char* Get_##_name() const \
{ \
	return getString(static_cast<uint32>(_field - 1)); \
}

#define __DBC_LOCSTR(_name, _field) \
const char* Get_##_name(int8 _locale = -1) const \
{ \
	return getLocalizedString(static_cast<uint32>(_field - 1), _locale); \
}

#define __DBC_REF_ID(_dbc, _name, _field) \
_dbc##Record* Get_##_name() const \
{ \
	return _dbc[static_cast<uint32>(getValue<uint32>(static_cast<uint32>(_field - 1)))]; \
}

#define __DBC_TARRAY(_type, _name, _field, _size) \
_type Get_##_name(uint8 _index) const \
{ \
    assert1(_index < _size); \
	return getValue<_type>(static_cast<uint32>(_field - 1 + _index)); \
}


// Record create

#define CONCAT_CLASS(a) a##Record

// Create class

#define DBC_DEF_BEGIN(accessName) \
class CONCAT_CLASS(accessName) : public Record { \
public: \
	CONCAT_CLASS(accessName)(const DBCFile<CONCAT_CLASS(accessName)>::Iterator& _iterator) : Record(_iterator->dbcStats, _iterator->offset) { } \
	CONCAT_CLASS(accessName)(DBCFile<CONCAT_CLASS(accessName)>* file, uint8* offset) : Record(file, offset) { } \
public:

#define DBC_DEF_END \
};

// Create accessor

#define DBC_DEFINE(accessName) \
class CONCAT_CLASS(accessName); \
extern DBCFile<CONCAT_CLASS(accessName)> accessName;

#define DBC_LOAD(accessName, fileName) DBCFile<CONCAT_CLASS(accessName)> accessName(fileName);



///////////////////////////////////
// DBC STATS
///////////////////////////////////
class Record;
class DBCStats
{
public:
	size_t getRecordSize() const { return recordSize; }
	size_t getRecordCount() const { return recordCount; }
	size_t getFieldCount() const { return fieldCount; }
	size_t getStringSize() const { return stringSize; }

protected:
	size_t recordSize;
	size_t recordCount;
	size_t fieldCount;
	size_t stringSize;

	// Strings
	uint8* stringTable;

	//

	friend Record;
};



///////////////////////////////////
// Record type
///////////////////////////////////
class Record
{
public:
	Record() = delete;
	Record(const Record& _record) = delete;
	Record(DBCStats* _dbcStats, uint8* offset) : dbcStats(_dbcStats), offset(offset) {}

	Record& operator=(const Record& r) = delete;

	// All data has ID
	__DBC_TVALUE(uint32, ID, 1);

protected:
	// Get value with common type
	template<typename T>
	T getValue(uint32 field) const
	{
		assert2(field < dbcStats->fieldCount, std::to_string(field).c_str());
		return *reinterpret_cast<T*>(offset + field * 4);
	}

	// Common types
	float_t getFloat(uint32 field) const
	{
		assert2(field < dbcStats->fieldCount, std::to_string(field).c_str());
		return *reinterpret_cast<float_t*>(offset + (field * 4));
	}
	uint32 getUInt(uint32 field) const
	{
		assert2(field < dbcStats->fieldCount, std::to_string(field).c_str());
		return *reinterpret_cast<uint32*>(offset + (field * 4));
	}
	int32 getInt(uint32 field) const
	{
		assert2(field < dbcStats->fieldCount, std::to_string(field).c_str());
		return *reinterpret_cast<int32*>(offset + (field * 4));
	}
	uint8 getByte(uint32 ofs) const
	{
		assert2(ofs < dbcStats->recordSize, std::to_string(ofs).c_str());
		return *reinterpret_cast<uint8*>(offset + ofs);
	}

	// Strings
	const char* getString(uint32 field) const
	{
		assert2(field < dbcStats->fieldCount, std::to_string(field).c_str());

		size_t stringOffset = getUInt(field);
		if (stringOffset >= dbcStats->stringSize)
		{
			stringOffset = 0;
		}

		assert2(stringOffset < dbcStats->stringSize, std::to_string(stringOffset).c_str());
		return reinterpret_cast<char*>(dbcStats->stringTable + stringOffset);
	}
	const char* getLocalizedString(uint32 field, int8 locale = -1) const
	{
		int8 loc = locale;
		if (locale == -1)
		{
			assert2(field < dbcStats->fieldCount - 8, std::to_string(field).c_str());
			for (loc = 0; loc < 8; loc++)
			{
				uint32 stringOffset = getUInt(field + loc);
				if (stringOffset != 0)
					break;
			}
		}
		assert2(field + loc < dbcStats->fieldCount, std::to_string(field).c_str());
		uint32 stringOffset = getUInt(field + static_cast<uint32>(loc));

		assert2(stringOffset < dbcStats->stringSize, std::to_string(stringOffset).c_str());
		return reinterpret_cast<char*>(dbcStats->stringTable + stringOffset);
	}

public:
	DBCStats* dbcStats;
	uint8* offset;
};



///////////////////////////////////
// DBC File
///////////////////////////////////
class File;
template <class RECORD_T>
class DBCFile : public File, public DBCStats
{
	friend RECORD_T;

public:
	DBCFile(cstring _file) : File(string("DBFilesClient\\") + string(_file)) {}
	~DBCFile() 
	{
		for (auto it = records.begin(); it != records.end();)
		{
			auto obj = it->second;
			it = records.erase(it);
			delete obj;
		}
	}

	// Open file and fill data
	bool Open();

	// Get data by id
	RECORD_T* operator[](uint32 _id);

	///////////////////////////////////
	// Iterator that iterates over records
	///////////////////////////////////
	class Iterator
	{
	public:
		Iterator(DBCFile* file, uint8* offset) : record(file, offset) {}

		Iterator& operator++()
		{
			record.offset += record.dbcStats->getRecordSize();
			return *this;
		}

		RECORD_T const& operator*() const
		{
			return record;
		}

		const RECORD_T* operator->() const
		{
			return &record;
		}

		bool operator==(const Iterator &b) const
		{
			return record.offset == b.record.offset;
		}

		bool operator!=(const Iterator &b) const
		{
			return record.offset != b.record.offset;
		}

	private:
		RECORD_T record;

		//
		friend RECORD_T;
	};

	// Iterators
	Iterator begin()
	{
		assert1(GetData() != nullptr);
		return Iterator(this, data + GetPos());
	}

	Iterator end()
	{
		assert1(GetData() != nullptr);
		return Iterator(this, stringTable);
	}

	const map<uint32, RECORD_T*>* Records() const
	{
		return &records;
	}

protected:
	map<uint32, RECORD_T*> records;
};

#include "DBC__File.inl"