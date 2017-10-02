#pragma once

class LocalFile
{
public:
	static  uint64_t GetFileSize(cstring _name);
	static  bool IsFileExists(cstring _name);

protected:
	 virtual bool OpenLocalFile() = 0;

public:
	 static const char* gamedata;
};