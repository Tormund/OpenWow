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

class MPQArchiveStorage
{
public:
	static void AddArchive(string filename);
	static MPQFileLocation GetFileLocation(cstring filename);
	static void ClearArchives();

	static void InitCommonArchives();

private:
	static vector<mpq_archive_s*> m_OpenArchives;
	static const char* archives;
};