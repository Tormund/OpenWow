#pragma once

// MPQ Files
#include <libmpq/mpq.h>

struct MPQFileLocation
{
	MPQFileLocation() : archive(nullptr), fileNumber(0), exists(false) {}
	MPQFileLocation(mpq_archive* _archive, uint32 _fileNumber) : archive(_archive), fileNumber(_fileNumber), exists(true) {}

	bool exists;
	mpq_archive* archive;
	uint32 fileNumber;
};

class MPQFile
{
public:
	static  uint64_t GetFileSize(cstring _name);
	static  bool IsFileExists(cstring _name);

protected:
	 virtual bool OpenMPQFile() = 0;

protected:
	static const char* archives;

public: // MPQ File
	static  void AddArchive(string filename);
	static  MPQFileLocation GetFileLocation(cstring filename);
	static  void ClearArchives();	
};