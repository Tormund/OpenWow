#pragma once

#include "BaseFile.h"
#include "LocalFile.h"
#include "MPQFile.h"

class File : public BaseFile, protected LocalFile, protected MPQFile
{
public:
	File();
	File(const File& _file);
	File(cstring _fullFileName);
	File(const char* _fullFileName);
	File(cstring _name, cstring _path);
	~File();

	//

	static void InitCriticalSect()
	{
		InitializeCriticalSection(&cs);
	}

	//

	File& operator=(const File& _file);
	File& operator=(cstring _fullFileName);
	File& operator=(const char* _fullFileName);

	//

	void SetName(cstring _fullFileName) override;
	void SetName(const char* _fullFileName) override;

	//

	string Name() const override;
	string Path() const override;
	string Extension() const override;
	string Path_Name() const override;

	string FullPath();

	//

	static  void SetDefaultFileLocation(FileLocation _fileLocation);

	//

	 virtual bool Open() override;

private:
	 bool OpenLocalFile() override;
	 bool OpenMPQFile() override;

private:
	static CRITICAL_SECTION cs;
	static FileLocation m_DefaultFileLocation;
};