#pragma once

// MPQ Files
#include <libmpq/mpq.h>
#pragma comment(lib, "libmpq.lib")

struct MPQFileLocation
{
	MPQFileLocation() : archive(nullptr), fileNumber(0), exists(false) {}
	MPQFileLocation(mpq_archive* _archive, uint32_t _fileNumber) : archive(_archive), fileNumber(_fileNumber), exists(true) {}

	bool exists;
	mpq_archive* archive;
	uint32_t fileNumber;
};

class MPQFile
{
public:
	static OW_CORE_DLL_API uint64_t GetFileSize(cstring _name);
	static OW_CORE_DLL_API bool IsFileExists(cstring _name);

protected:
	OW_CORE_DLL_API virtual bool OpenMPQFile() = 0;

protected:
	static const char* archives;

public: // MPQ File
	static OW_CORE_DLL_API void AddArchive(string filename);
	static OW_CORE_DLL_API MPQFileLocation GetFileLocation(cstring filename);
	static OW_CORE_DLL_API void ClearArchives();	
};