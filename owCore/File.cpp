#include "stdafx.h"

// General
#include "File.h"

bool File::usePackedGamedata = false;
const char* File::archives = "D:/_games/World of Warcraft 4.3.4/Data/";
const char* File::gamedata = "D:/_games/World of Warcraft 4.3.4 ExData/";

File::File() :
	ByteBuffer(),
	name(""),
	path(""),
	extension("")
{}

File::File(const File& _file) :
	ByteBuffer(_file), name(_file.name),
	path(_file.path),
	extension(_file.extension)
{}

File::File(const ByteBuffer& _buffer) :
	ByteBuffer(_buffer),
	name("<empty>"),
	path(""),
	extension("")
{}

File::File(cstring _fullFileName) :
	name(_fullFileName),
	path("")
{
	ParsePathAndExtension();
}

File::File(const char* _fullFileName) :
	name(_fullFileName),
	path("")
{
	ParsePathAndExtension();
}

File::File(cstring _name, cstring _path) :
	name(_name),
	path(_path)
{
	ParsePathAndExtension();
}

File::~File()
{
	ByteBuffer::~ByteBuffer();
}

// Equal operators copy only name and path

File& File::operator=(const File& _file)
{
	name = _file.name;
	path = _file.path;
	extension = _file.extension;

	return *this;
}

File& File::operator=(cstring _fullFileName)
{
	name = _fullFileName;

	ParsePathAndExtension();

	return *this;
}

File& File::operator=(const char* _fullFileName)
{
	name = _fullFileName;

	ParsePathAndExtension();

	return *this;
}

//

bool File::Open(bool _isLocalFile)
{
	return OpenLocalFile();
}

size_t File::getSize(cstring _name)
{
	// Open stream
	ifstream stream;
	stream.open(string(gamedata + _name), ios::binary);

	// Check stream
	if (!stream.is_open())
	{
		return 0;
	}

	// Filesize
	stream.seekg(0, stream.end);
	size_t fileSize = size_t(stream.tellg());
	stream.seekg(0, stream.beg);

	stream.clear();
	stream.close();

	return fileSize;
}

bool File::exists(cstring _name)
{
	// Open stream
	ifstream stream;
	stream.open(string(gamedata + _name), ios::binary);

	// Check stream
	if (!stream.is_open())
		return false;

	stream.clear();
	stream.close();

	return true;
}

// 

bool File::OpenLocalFile()
{
	if (ByteBuffer::isFilled)
	{
		Debug::Warn("File[%s]: Not reason to open file, because buffer is filled.", Path_Name().c_str());
		return true;
	}

	// Open stream
	ifstream stream;
	stream.clear();
	stream.open(File::gamedata + path + name, ios::binary);


	// Check stream
	if (!stream.is_open())
	{
		Debug::Error("File[%s]: Can not open file!", Path_Name().c_str());
		return false;
	}

	// Filesize
	stream.seekg(0, stream.end);
	size_t fileSize = size_t(stream.tellg());
	stream.seekg(0, stream.beg);

	// Check filesize
	if (fileSize == 0)
	{
		Debug::Error("File[%s]: Is empty!", Path_Name().c_str());
		return false;
	}

	// Read data
	ByteBuffer::Allocate(fileSize);
	stream.read((char*)&ByteBuffer::GetData()[0], fileSize);
	isFilled = true;

	streamsize readedBytes = stream.gcount();
	if (readedBytes < fileSize)
	{
		memset(&data[0] + readedBytes, 0, fileSize - static_cast<size_t>(readedBytes));
		Debug::Error("File[%s]: Stream reading error. Readed [%d], filesize [%d]", Path_Name().c_str(), readedBytes, fileSize);
	}

	// Close stream
	stream.close();
	stream.clear();

	return true;
}

void File::ParsePathAndExtension()
{
	auto lastSlashPos = name.find_last_of('/');
	if (lastSlashPos != string::npos)
	{
		path += name.substr(0, lastSlashPos + 1);
		name = name.substr(lastSlashPos + 1);
	}

	auto lastPointPos = name.find_last_of('.');
	if (lastPointPos != string::npos)
		extension = Utils::ToLower(name.substr(lastPointPos + 1));
}