#include "stdafx.h"

// General
#include "DBCFile.h"

/*bool DBCFile::Open(cstring _name) {
	int db_type = 0;

	if(!File::Open()) {
		Debug::Error("DBCFile[%s]: Can't open.", Path_Name().c_str());
		return false;
	}

	char header[5];
	ReadBytes(header, 4);
	header[4] = 0;

	if(strncmp(header, "WDBC", 4) == 0) {
		db_type = 1;
		Debug::Print("DBCFile[%s]: File header [%s]. Type [%d].", Path_Name().c_str(), header, db_type);
	}
	else if(strncmp(header, "WDB2", 4) == 0) {
		db_type = 2;
		Debug::Print("DBCFile[%s]: File header [%s]. Type [%d].", Path_Name().c_str(), header, db_type);
	}
	else {
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

	if(db_type == 2) {
		SeekRelative(28);
		// just some buggy check
		uint32 check;
		ReadBytes(&check, 4);
		if(check == 6) // wrong place
			SeekRelative(-20);
		else // check == 17, right place
			SeekRelative(-4);
	}

	assert1(fieldCount * 4 >= recordSize); // not always true, but it works fine till now //assert1(fieldCount*4 == recordSize);

	// Pointer to data
	stringTable = GetPos() + GetData() + recordSize * recordCount;

	for(auto _offset = GetData() + GetPos(); _offset != stringTable; _offset += recordSize) {
		Record* record = new Record(this, _offset);
		records.insert(make_pair(record->getUInt(0), record));
	}

	return true;
}*/


