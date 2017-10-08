#pragma once

template <class RECORD_T>
bool DBCFile<RECORD_T>::Open()
{
	// Try open file
	if (!File::Open())
	{
		Debug::Error("DBCFile[%s]: Can't open file.", Path_Name().c_str());
		return false;
	}

	char header[5];
	ReadBytes(header, 4);
	header[4] = 0;

	int db_type = 0;
	if (strncmp(header, "WDBC", 4) == 0)
	{
		db_type = 1;
	}
	else if (strncmp(header, "WDB2", 4) == 0)
	{
		db_type = 2;
	}
	else
	{
		Debug::Error("DBCFile[%s]: File corrupt. Header [%s]", Path_Name().c_str(), header);
		return false;
	}

	Debug::Print("DBCFile[%s]: File header [%s]. Type [%d].", Path_Name().c_str(), header, db_type);

	ByteBuffer::ReadBytes(&recordCount, 4);// Number of records
	ByteBuffer::ReadBytes(&fieldCount, 4); // Number of fields
	ByteBuffer::ReadBytes(&recordSize, 4); // Size of a record
	ByteBuffer::ReadBytes(&stringSize, 4); // String size

	if (db_type == 2)
	{
		SeekRelative(28);

		unsigned int check;
		ByteBuffer::ReadBytes(&check, 4);
		if (check == 6)
		{
			ByteBuffer::SeekRelative(-20);
		}
		else // 17
		{
			ByteBuffer::SeekRelative(-4);
		}
	}

	assert1(fieldCount * 4 >= recordSize); // not always true, but it works fine till now


	stringTable = GetPos() + data + recordSize * recordCount;

	// Fill record table
	for (auto _offset = data + GetPos(); _offset != stringTable; _offset += recordSize)
	{
		RECORD_T* record = new RECORD_T(this, _offset);
		records.insert(make_pair(record->Get_ID(), record));
	}

	return true;
}

template <class RECORD_T>
RECORD_T* DBCFile<RECORD_T>::operator[](uint32 _id)
{
	auto recordIt = records.find(_id);
	if (recordIt != records.end())
	{
		return recordIt->second;
	}

	return nullptr;
}