#pragma once

class LocalFile
{
public:
	static OW_CORE_DLL_API uint64_t GetFileSize(cstring _name);
	static OW_CORE_DLL_API bool IsFileExists(cstring _name);

protected:
	OW_CORE_DLL_API virtual bool OpenLocalFile() = 0;

protected:
	static const char* gamedata;
};



