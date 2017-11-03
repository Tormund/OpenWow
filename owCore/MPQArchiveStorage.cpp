#include "stdafx.h"

// General
#include "MPQArchiveStorage.h"

const char* MPQArchiveStorage::archives = "D:/_games/World of Warcraft 4.3.4/Data/";
vector<mpq_archive_s*> MPQArchiveStorage::m_OpenArchives;

void MPQArchiveStorage::AddArchive(string filename)
{
	mpq_archive_s* mpq_a;
	int result = libmpq__archive_open(&mpq_a, (MPQArchiveStorage::archives + filename).c_str(), -1);
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

MPQFileLocation MPQArchiveStorage::GetFileLocation(cstring filename)
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

void MPQArchiveStorage::ClearArchives()
{
	for (auto it : m_OpenArchives)
	{
		libmpq__archive_close(it);
	}
}

void MPQArchiveStorage::InitCommonArchives()
{
	// Files
	AddArchive(string("art.MPQ"));
	AddArchive(string("base-Win.MPQ"));
	AddArchive(string("expansion1.MPQ"));
	AddArchive(string("expansion2.MPQ"));
	AddArchive(string("expansion3.MPQ"));
	//AddArchive(string("sound.MPQ"));
	AddArchive(string("world.MPQ"));
	AddArchive(string("world2.MPQ"));

	//AddArchive(string("wow-update-base-15211.MPQ"));
	//AddArchive(string("wow-update-base-15354.MPQ"));
	//AddArchive(string("wow-update-base-15595.MPQ"));

	AddArchive(string("enUS/expansion1-locale-enUS.MPQ"));
	//AddArchive(string("enUS/expansion1-speech-enUS.MPQ"));
	AddArchive(string("enUS/expansion2-locale-enUS.MPQ"));
	//AddArchive(string("enUS/expansion2-speech-enUS.MPQ"));
	AddArchive(string("enUS/expansion3-locale-enUS.MPQ"));
	//AddArchive(string("enUS/expansion3-speech-enUS.MPQ"));
	AddArchive(string("enUS/locale-enUS.MPQ"));
	//AddArchive(string("enUS/speech-enUS.MPQ"));

	//AddArchive(string("enUS/wow-update-enUS-15211.MPQ"));
	//AddArchive(string("enUS/wow-update-enUS-15354.MPQ"));
	//AddArchive(string("enUS/wow-update-enUS-15595.MPQ"));
}
