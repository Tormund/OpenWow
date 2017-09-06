#pragma once

#include "ByteBuffer.h"

class BaseFile : public ByteBuffer
{
public:
	OW_CORE_DLL_API BaseFile();
	OW_CORE_DLL_API BaseFile(const BaseFile& _file);
	OW_CORE_DLL_API BaseFile(const ByteBuffer& _buffer);
	OW_CORE_DLL_API BaseFile(cstring _fullFileName);
	OW_CORE_DLL_API BaseFile(const char* _fullFileName);
	OW_CORE_DLL_API BaseFile(cstring _name, cstring _path);
	OW_CORE_DLL_API ~BaseFile();

	//

	OW_CORE_DLL_API virtual bool Open() = 0;

protected:
	void ParsePathAndExtension();

protected:
	string name;
	string path;
	string extension;
};