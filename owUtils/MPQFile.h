#pragma once

class MPQFile
{
public:
	static uint64_t GetFileSize(cstring _name);
	static bool IsFileExists(cstring _name);

protected:
	virtual bool OpenMPQFile() = 0;
};