#pragma once

// Add data

#define ADD_TYPE_VALUE(type, name, field) \
type Get_##name() const { \
	return getValue<type>(static_cast<uint32_t>(field)); \
}

#define ADD_STRING(name, field) \
const char* Get_##name##_cstr() const { \
	return getString(static_cast<uint32_t>(field)); \
}

#define ADD_LOCALE_STRING(name, field) \
const char* Get_##name##_cstr(int8_t _locale = -1) const { \
	return getLocalizedString(static_cast<uint32_t>(field), _locale); \
}

// Record create

#define CONCAT(a, b) a##b
#define CONCAT_CLASS(a) a##Record

#define __DBC_DEF_BEGIN(accessName) \
class CONCAT_CLASS(accessName); \
 extern DBCFile<CONCAT_CLASS(accessName)> accessName; \
class CONCAT_CLASS(accessName) : public Record { \
public: \
	CONCAT_CLASS(accessName)(const DBCFile<CONCAT_CLASS(accessName)>::Iterator& _iterator) : Record(_iterator->dbcStats, _iterator->offset) { } \
	CONCAT_CLASS(accessName)(DBCFile<CONCAT_CLASS(accessName)>* file, uint8_t* offset) : Record(file, offset) { } \
public:

#define __DBC_DEF_END };

#define __DBC_LOAD(accessName, fileName) DBCFile<CONCAT_CLASS(accessName)> accessName(fileName);



///////////////////////////////////
// DBC STATS
///////////////////////////////////
class Record;
class DBCStats
{
	friend Record;
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

	// WDB2
	uint32_t tableHash;
	uint32_t build;
	uint32_t timestampLastWritten;
	uint32_t minId;
	uint32_t maxId;
	uint32_t locale;
	uint32_t copyTableSize;

	// Strings
	uint8_t* stringTable;
};



///////////////////////////////////
// Record type
///////////////////////////////////
class Record
{
public:
	Record(DBCStats* _dbcStats, uint8_t* offset) : dbcStats(_dbcStats), offset(offset) {}

public:
	Record& operator=(const Record& r) = delete;

	// All data has ID
	ADD_TYPE_VALUE(uint32_t, ID, 0);

	// Get value with common type
	template<typename T>
	T getValue(uint32_t field) const
	{
		assert2(field < dbcStats->fieldCount, std::to_string(field).c_str());
		return *reinterpret_cast<T*>(offset + field * 4);
	}

	float_t getFloat(uint32_t field) const
	{
		assert2(field < dbcStats->fieldCount, std::to_string(field).c_str());
		return *reinterpret_cast<float*>(offset + field * 4);
	}
	uint32_t getUInt(uint32_t field) const
	{
		assert2(field < dbcStats->fieldCount, std::to_string(field).c_str());
		return *reinterpret_cast<unsigned int*>(offset + (field * 4));
	}
	int32_t getInt(uint32_t field) const
	{
		assert2(field < dbcStats->fieldCount, std::to_string(field).c_str());
		return *reinterpret_cast<int*>(offset + field * 4);
	}

	// Get uint8_t
	uint8_t getByte(uint32_t ofs) const
	{
		assert2(ofs < dbcStats->recordSize, std::to_string(ofs).c_str());
		return *reinterpret_cast<uint8_t*>(offset + ofs);
	}

	// Strings
	const char* getString(uint32_t field) const
	{
		assert2(field < dbcStats->fieldCount, std::to_string(field).c_str());

		size_t stringOffset = getUInt(field);
		if (stringOffset >= dbcStats->stringSize)
			stringOffset = 0;

		assert2(stringOffset < dbcStats->stringSize, std::to_string(stringOffset).c_str());
		return reinterpret_cast<char*>(dbcStats->stringTable + stringOffset);
	}
	const char* getLocalizedString(uint32_t field, int8_t locale = -1) const
	{
		int8_t loc = locale;
		if (locale == -1)
		{
			assert2(field < dbcStats->fieldCount - 8, std::to_string(field).c_str());
			for (loc = 0; loc < 8; loc++)
			{
				uint32_t stringOffset = getUInt(field + loc);
				if (stringOffset != 0)
					break;
			}
		}
		assert2(field + loc < dbcStats->fieldCount, std::to_string(field).c_str());
		uint32_t stringOffset = getUInt(field + static_cast<uint32_t>(loc));

		assert2(stringOffset < dbcStats->stringSize, std::to_string(stringOffset).c_str());
		return reinterpret_cast<char*>(dbcStats->stringTable + stringOffset);
	}

public:
	DBCStats* dbcStats;
	uint8_t* offset;
};


// Exceptions
class DBCException
{
public:
	DBCException(const string &message) : message(message) {}
	virtual ~DBCException() {}

	cstring getMessage() { return message; }
private:
	string message;
};

class DBCNotFound : public DBCException
{
public:
	DBCNotFound() : DBCException("Key was not found") {}
};


///////////////////////////////////
// DBC File
///////////////////////////////////
class File;
template <class TT>
class DBCFile : public File, public DBCStats
{
	friend TT;
public:
	DBCFile(const File& _file) : File(_file) {}

	// Open file and fill data
	bool Open()
	{
		int db_type = 0;

		// Tty open file
		if (!File::Open())
		{
			Debug::Error("DBCFile[%s]: Can't open file.", Path_Name().c_str());
			return false;
		}

		char header[5];
		ReadBytes(header, 4);
		header[4] = 0;

		if (strncmp(header, "WDBC", 4) == 0)
		{
			db_type = 1;
			Debug::Print("DBCFile[%s]: File header [%s]. Type [%d].", Path_Name().c_str(), header, db_type);
		}
		else if (strncmp(header, "WDB2", 4) == 0)
		{
			db_type = 2;
			Debug::Print("DBCFile[%s]: File header [%s]. Type [%d].", Path_Name().c_str(), header, db_type);
		}
		else
		{
			Debug::Error("DBCFile[%s]: File corrupt. Header [%s]", Path_Name().c_str(), header);
			return false;
		}

		ReadBytes(&recordCount, 4);// Number of records
		ReadBytes(&fieldCount, 4); // Number of fields
		ReadBytes(&recordSize, 4); // Size of a record
		ReadBytes(&stringSize, 4); // String size

		// WDB2
		//ReadBytes(&tableHash, 4);
		//ReadBytes(&build, 4);
		//ReadBytes(&timestampLastWritten, 4);
		//ReadBytes(&minId, 4);
		//ReadBytes(&maxId, 4);
		//ReadBytes(&locale, 4);
		//ReadBytes(&copyTableSize, 4);

		//Debug::Info("DBCFile[%s]: TableHash [%d], build [%d], TimestampLastWritten [%d]", Path_Name().c_str(), tableHash, build, timestampLastWritten);
		//Debug::Info("DBCFile[%s]: MinId [%d], MaxId [%d] Locale [%d], CopyTableSize [%d]", Path_Name().c_str(), minId, maxId, locale, copyTableSize);

		if (db_type == 2)
		{
			SeekRelative(28);

			unsigned int check;
			ReadBytes(&check, 4);
			if (check == 6)
				SeekRelative(-20);
			else // 17
				SeekRelative(-4);
		}

		assert1(fieldCount * 4 >= recordSize); // not always true, but it works fine till now //assert1(fieldCount*4 == recordSize);

		// Pointer to data
		stringTable = GetPos() + data + recordSize * recordCount;

		// Fill record table
		for (auto _offset = data + GetPos(); _offset != stringTable; _offset += recordSize)
		{
			TT* record = new TT(this, _offset);
			records.insert(make_pair(record->getUInt(0), record));
		}

		return true;
	}

	///////////////////////////////////
	// Iterator that iterates over records
	///////////////////////////////////
	class Iterator
	{
		friend TT;
	public:
		Iterator(DBCFile* file, uint8_t* offset) : record(file, offset) {}

		Iterator& operator++()
		{
			record.offset += record.dbcStats->getRecordSize();
			return *this;
		}

		TT const& operator*() const
		{
			return record;
		}

		const TT* operator->() const
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
		TT record;
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

	const map<uint32_t, TT*>* Records() const
	{
		return &records;
	}

	// Get data by ID
	TT* getByID(uint32_t _id)
	{
		auto recordIt = records.find(_id);
		if (recordIt != records.end())
			return recordIt->second;

		throw DBCNotFound();
	}

protected:
	map<uint32_t, TT*> records;
};