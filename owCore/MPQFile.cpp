#include "stdafx.h"

// General
#include "MPQFile.h"

// Additional
#include "MPQArchiveStorage.h"

uint64_t MPQFile::GetFileSize(cstring _name)
{
	MPQFileLocation location = MPQArchiveStorage::GetFileLocation(_name);

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
	return MPQArchiveStorage::GetFileLocation(_name).exists;
}


