#include "stdafx.h"

// General
#include "File.h"

File::File() : BaseFile()
{}

File::File(const File& _file) : BaseFile(_file)
{}

File::File(cstring _fullFileName) : BaseFile(_fullFileName)
{}

File::File(const char* _fullFileName) : BaseFile(_fullFileName)
{}

File::File(cstring _name, cstring _path) : BaseFile(_name, _path)
{}

File::~File()
{
	BaseFile::~BaseFile();
}

//

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

void File::SetName(cstring _fullFileName)
{
	name = _fullFileName;

	ParsePathAndExtension();
}

void File::SetName(const char * _fullFileName)
{
	name = _fullFileName;

	ParsePathAndExtension();
}

//

string File::Name() const
{
	return name;
}
string File::Path() const
{
	return path;
}
string File::Extension() const
{
	return extension;
}

string File::Path_Name() const
{
	return string(path + name);
}

bool File::Open()
{
	if (OpenMPQFile())
	{
		return true;
	}

	if (OpenLocalFile())
	{
		return true;
	}

	return false;
}

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
	stream.open(LocalFile::gamedata + Path_Name(), ios::binary);


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

bool File::OpenMPQFile()
{
	MPQFileLocation location = GetFileLocation(Path_Name());

	if (location.exists)
	{
		libmpq__off_t size;
		libmpq__file_size_unpacked(location.archive, location.fileNumber, &size);

		// HACK: in patch.mpq some files don't want to open and give 1 for filesize
		if (size <= 1)
		{
			Debug::Warn("MPQFile[%s]: Has size [%d]. Considered dummy file.", Path_Name().c_str(), size);
			isEof = true;
			data = 0;
			return false;
		}

		// Allocate space and set data
		ByteBuffer::Allocate(size);
		libmpq__file_read(location.archive, location.fileNumber, data, bufferSize, &size);
		assert1(bufferSize == size);
		ByteBuffer::Init(data, size);

		return true;
	}

	return false;
}

