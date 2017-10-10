#include "stdafx.h"

// General
#include "MPQFile.h"

const char* MPQFile::archives = "D:/_games/World of Warcraft 4.3.4/Data/";

uint64_t MPQFile::GetFileSize(cstring _name)
{
	MPQFileLocation location = GetFileLocation(_name);

	if (location.exists)
	{
		libmpq__off_t size;
		libmpq__file_size_unpacked(location.archive, location.fileNumber, &size);
		return size;
	}
	
	return 0;
}

bool MPQFile::IsFileExists(cstring _name)
{
	return GetFileLocation(_name).exists;
}

//

vector<mpq_archive_s*> m_OpenArchives;

void MPQFile::AddArchive(string filename)
{
	mpq_archive_s* mpq_a;
	int result = libmpq__archive_open(&mpq_a, (MPQFile::archives + filename).c_str(), -1);
	Modules::log().Info("Opening %s", filename.c_str());
	if (result)
	{
		switch (result)
		{
			case LIBMPQ_ERROR_OPEN:
			Modules::log().Error("Error opening archive [%s]: Does file really exist?", filename.c_str());
			break;

			case LIBMPQ_ERROR_FORMAT:            /* bad file format */
			Modules::log().Error("Error opening archive [%s]: Bad file format", filename.c_str());
			break;

			case LIBMPQ_ERROR_SEEK:         /* seeking in file failed */
			Modules::log().Error("Error opening archive [%s]: Seeking in file failed", filename.c_str());
			break;

			case LIBMPQ_ERROR_READ:              /* Read error in archive */
			Modules::log().Error("Error opening archive [%s]: Read error in archive", filename.c_str());
			break;

			case LIBMPQ_ERROR_MALLOC:               /* maybe not enough memory? :) */
			Modules::log().Error("Error opening archive [%s]: Maybe not enough memory", filename.c_str());
			break;

			default:
			Modules::log().Error("Error opening archive [%s]: Unknown error\n", filename.c_str());
			break;
		}
		return;
	}


	/*uint32 filenum;
	if (libmpq__file_number(mpq_a, "(listfile)", &filenum)) return;

	libmpq__off_t size, transferred;
	libmpq__file_size_unpacked(mpq_a, filenum, &size);

	char *buffer = new char[size + 1];
	buffer[size] = '\0';

	libmpq__file_read(mpq_a, filenum, (unsigned char*)buffer, size, &transferred);

	char seps[] = "\n";
	char *token;

	strtok_s(buffer, seps, &token);
	uint32 counter = 0;
	while ((token != NULL) && (counter < size))
	{
		//cout << token << endl;
		token[strlen(token) - 1] = 0;
		std::string s = token;

		//filelist.push_back(s);

		//if(s[0] == 'I')
		//	Modules::log().Info("File [%s]", s.c_str());

		counter += strlen(token) + 2;
		strtok_s(NULL, seps, &token);
	}

	delete[] buffer;*/

	m_OpenArchives.push_back(mpq_a);
	Modules::log().Green("MPQFile[%s]: Added!", filename.c_str());
}

MPQFileLocation MPQFile::GetFileLocation(cstring filename)
{
	for (auto i = m_OpenArchives.begin(); i != m_OpenArchives.end(); ++i)
	{
		mpq_archive_s* mpq_a = *i;

		uint32 filenum;
		if (libmpq__file_number(mpq_a, filename.c_str(), &filenum) == LIBMPQ_ERROR_EXIST)
		{
			continue;
		}

		return MPQFileLocation(mpq_a, filenum);
	}

	return MPQFileLocation();
}

void MPQFile::ClearArchives()
{
	for (auto it = m_OpenArchives.begin(); it != m_OpenArchives.end(); ++it)
	{
		libmpq__archive_close(*it);
	}
}


