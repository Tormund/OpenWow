#include "stdafx.h"

// General
#include "BaseFile.h"

BaseFile::BaseFile() :
	ByteBuffer(),
	name(""),
	path(""),
	extension("")
{}

BaseFile::BaseFile(const BaseFile& _file) :
	ByteBuffer(_file),
	name(_file.name),
	path(_file.path),
	extension(_file.extension)
{}

BaseFile::BaseFile(const ByteBuffer& _buffer) :
	ByteBuffer(_buffer),
	name("<empty>"),
	path(""),
	extension("")
{}

BaseFile::BaseFile(cstring _fullFileName) :
	name(_fullFileName),
	path("")
{
	ParsePathAndExtension();
}

BaseFile::BaseFile(const char* _fullFileName) :
	name(_fullFileName),
	path("")
{
	ParsePathAndExtension();
}

BaseFile::BaseFile(cstring _name, cstring _path) :
	name(_name),
	path(_path)
{
	ParsePathAndExtension();
}

BaseFile::~BaseFile()
{
}

void BaseFile::ParsePathAndExtension()
{
	// Replace slahes
	size_t index = 0;
	while (true)
	{
		index = name.find('/', index);
		if (index == std::string::npos)
		{
			break;
		}

		name.replace(index, 1, "\\");

		index += 3;
	}

	// Find name and path
	auto lastSlashPos = name.find_last_of('\\');
	if (lastSlashPos != string::npos)
	{
		path += name.substr(0, lastSlashPos + 1);
		name = name.substr(lastSlashPos + 1);
	}

	auto lastPointPos = name.find_last_of('.');
	if (lastPointPos != string::npos)
	{
		extension = Utils::ToLower(name.substr(lastPointPos + 1));
	}
}