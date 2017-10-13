#include "stdafx.h"

// General
#include "File.h"

FileLocation File::m_DefaultFileLocation = FL_Any;;
CRITICAL_SECTION File::cs;

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
	//Modules::log().Error("File[%s] closed.", Path_Name().c_str());
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

//

string File::FullPath()
{
	return LocalFile::gamedata + path + name;
}

/*mpq_archive* File::MPQArchive()
{
	return nullptr;
}*/

void File::SetDefaultFileLocation(FileLocation _fileLocation)
{
	m_DefaultFileLocation = _fileLocation;
}

bool File::Open()
{
	EnterCriticalSection(&cs); // THREAD

	bool result = false;

	switch (m_DefaultFileLocation)
	{
		case FL_Any:
		{
			if (OpenMPQFile())
			{
				result = true;
			}
			else if (OpenLocalFile())
			{
				result = true;
			}
		}
		break;

		case FL_Local:
		{
			if (OpenLocalFile())
			{
				result = true;
			}
			else if (OpenMPQFile())
			{
				result = true;
			}
		}
		break;

		case FL_Local_Only:
		{
			if (OpenLocalFile())
			{
				result = true;
			}
		}
		break;

		case FL_MPQ_Only:
		{
			if (OpenMPQFile())
			{
				result = true;
			}
		}
		break;
	}

	LeaveCriticalSection(&cs); // THREAD

	return result;
}

bool File::OpenLocalFile()
{
	if (ByteBuffer::isFilled)
	{
		Modules::log().Warn("File[%s]: Not reason to open file, because buffer is filled.", Path_Name().c_str());
		return true;
	}

	// Open stream
	ifstream stream;
	stream.clear();

	if (Path_Name().find_first_of(':') != -1)
	{
		stream.open(Path_Name(), ios::binary);
	}
	else
	{
		stream.open(LocalFile::gamedata + Path_Name(), ios::binary);
	}

	// Check stream
	if (!stream.is_open())
	{
		Modules::log().Error("File[%s]: Can not open file!", Path_Name().c_str());
		return false;
	}

	// Filesize
	stream.seekg(0, stream.end);
	size_t fileSize = size_t(stream.tellg());
	stream.seekg(0, stream.beg);

	// Check filesize
	if (fileSize == 0)
	{
		Modules::log().Error("File[%s]: Is empty!", Path_Name().c_str());
		return false;
	}

	// Read data
	ByteBuffer::Allocate(fileSize + 1);
	stream.read((char*)&data[0], fileSize);
	ByteBuffer::SetFilled();

	streamsize readedBytes = stream.gcount();
	if (readedBytes < fileSize)
	{
		memset(&data[0] + readedBytes, 0, fileSize - static_cast<size_t>(readedBytes));
		Modules::log().Error("File[%s]: Stream reading error. Readed [%d], filesize [%d]", Path_Name().c_str(), readedBytes, fileSize);
		fail1();
	}

	data[fileSize] = '\0';

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
			Modules::log().Warn("MPQFile[%s]: Has size [%d]. Considered dummy file.", Path_Name().c_str(), size);
			isEof = true;
			data = nullptr;
			return false;
		}

		// Allocate space and set data
		ByteBuffer::Allocate(size);
		libmpq__file_read(location.archive, location.fileNumber, data, bufferSize, &size);
		assert1(bufferSize == size);
		ByteBuffer::SetFilled();

		return true;
	}

	return false;
}

