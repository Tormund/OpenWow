#include "stdafx.h"

// General
#include "MPQFile.h"

std::vector<mpq_archive*> gOpenArchives;

//

MPQFile::MPQFile(const MPQFile& _file) : File(_file)
{}

MPQFile::MPQFile(cstring _fullFileName) : File(_fullFileName)
{}

MPQFile::MPQFile(const char* _fullFileName) : File(_fullFileName)
{}

MPQFile::~MPQFile()
{
	File::~File();
}

bool MPQFile::Open(bool _isLocalFile)
{
	for (auto i = gOpenArchives.begin(); i != gOpenArchives.end(); ++i)
	{
		mpq_archive &mpq_a = **i;
		int fileno = libmpq_file_number(&mpq_a, Path_Name().c_str());
		if (fileno == LIBMPQ_EFILE_NOT_FOUND)
		{
			continue;
		}

		// Found!
		bufferSize = libmpq_file_info(&mpq_a, LIBMPQ_FILE_UNCOMPRESSED_SIZE, fileno);

		// HACK: in patch.mpq some files don't want to open and give 1 for filesize
		if (bufferSize <= 1)
		{
			isEof = true;
			data = 0;
			return true;
		}

		data = new uint8_t[bufferSize];
		libmpq_file_getdata(&mpq_a, fileno, (unsigned char*)data);

		return true;
	}

	isEof = true;
	data = 0;

	return true;
}

void MPQFile::AddArchive(cstring filename)
{
	mpq_archive mpq_a;
	int result = libmpq_archive_open(&mpq_a, (uint8_t*)(File::archives + filename).c_str());
	Debug::Green("Opening [%s]", filename);
	if (result)
	{
		Debug::Green("Error opening archive [%s]", filename);
		return;
	}
	gOpenArchives.push_back(&mpq_a);
}

void MPQFile::ClearArchives()
{
	for (auto i = gOpenArchives.begin(); i != gOpenArchives.end(); ++i)
	{
		libmpq_archive_close(*i);
	}
}


