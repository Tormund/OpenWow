#include "stdafx.h"

// General
#include "LocalFile.h"

const char* LocalFile::gamedata = "D:\\_programming\\OpenWow\\_gamedata\\";

uint64_t LocalFile::GetFileSize(cstring _name)
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
	uint64_t fileSize = static_cast<uint64_t>(stream.tellg());
	stream.seekg(0, stream.beg);

	stream.clear();
	stream.close();

	return fileSize;
}

bool LocalFile::IsFileExists(cstring _name)
{
	// Open stream
	ifstream stream;
	stream.open(string(gamedata + _name), ios::binary);

	// Check stream
	if (!stream.is_open())
	{
		return false;
	}

	stream.clear();
	stream.close();

	return true;
}
