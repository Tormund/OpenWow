#pragma once

class MPQFile : public File
{
public:
	OW_CORE_DLL_API MPQFile(const MPQFile& _file);
	OW_CORE_DLL_API MPQFile(cstring _fullFileName);
	OW_CORE_DLL_API MPQFile(const char* _fullFileName);
	~MPQFile();

	OW_CORE_DLL_API bool Open(bool _isLocalFile = false) override;

public:
	static OW_CORE_DLL_API void AddArchive(cstring filename);
	static OW_CORE_DLL_API void ClearArchives();
};

