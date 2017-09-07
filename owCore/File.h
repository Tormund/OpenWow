#pragma once

#include "BaseFile.h"
#include "LocalFile.h"
#include "MPQFile.h"

class File : public BaseFile, protected LocalFile, protected MPQFile
{
public:
	OW_CORE_DLL_API File();
	OW_CORE_DLL_API File(const File& _file);
	OW_CORE_DLL_API File(cstring _fullFileName);
	OW_CORE_DLL_API File(const char* _fullFileName);
	OW_CORE_DLL_API File(cstring _name, cstring _path);
	OW_CORE_DLL_API ~File();

	OW_CORE_DLL_API File& operator=(const File& _file);
	OW_CORE_DLL_API File& operator=(cstring _fullFileName);
	OW_CORE_DLL_API File& operator=(const char* _fullFileName);

	//

	OW_CORE_DLL_API void SetName(cstring _fullFileName) override;
	OW_CORE_DLL_API void SetName(const char* _fullFileName) override;

	//

	OW_CORE_DLL_API string Name() const override;
	OW_CORE_DLL_API string Path() const override;
	OW_CORE_DLL_API string Extension() const override;

	OW_CORE_DLL_API string Path_Name() const override;

	//

	OW_CORE_DLL_API bool Open() override;

private:
	OW_CORE_DLL_API bool OpenLocalFile() override;
	OW_CORE_DLL_API bool OpenMPQFile() override;
};