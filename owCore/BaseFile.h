#pragma once

#include "ByteBuffer.h"

enum FileLocation
{
	FL_Any = 0,
	FL_MPQ_Only = 3,
	FL_Local = 2,
	FL_Local_Only = 4
};

class BaseFile : public ByteBuffer
{
public:
	 BaseFile();
	 BaseFile(const BaseFile& _file);
	 BaseFile(const ByteBuffer& _buffer);
	 BaseFile(cstring _fullFileName);
	 BaseFile(const char* _fullFileName);
	 BaseFile(cstring _name, cstring _path);
	 ~BaseFile();

	//

	 virtual void SetName(cstring _fullFileName) = 0;
	 virtual void SetName(const char* _fullFileName) = 0;

	//

	 virtual string Name() const = 0;
	 virtual string Path() const = 0;
	 virtual string Extension() const = 0;

	 virtual string Path_Name() const = 0;

	//

	 virtual bool Open() = 0;

protected:
	void ParsePathAndExtension();

protected:
	string name;
	string path;
	string extension;
};