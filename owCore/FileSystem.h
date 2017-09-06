#pragma once

class Module;

#include "File.h"

class FileSystem : public Module
{
private:
	struct SectionData
	{
		map<string, string> data;
	};
	typedef map<string, SectionData*> fsDataType;

public:
	DEF_MODULE(FileSystem, OW_CORE_DLL_API);

	OW_CORE_DLL_API void Print();
	OW_CORE_DLL_API string Get(cstring _sectionName, cstring _keyName) const;

	template<typename T>
	bool Assign(T& addr, cstring _sectionName, cstring _keyName);

	OW_CORE_DLL_API bool IsSectionExists(cstring _sectionName) const { return SectionExists(data, _sectionName); }
	OW_CORE_DLL_API bool IsKeyExists(cstring _sectionName, cstring _keyName)  const;

private:
	bool Open(const File& _file, fsDataType& _currentData, bool _debugMode);

	bool SectionExists(const fsDataType& _data, cstring _sectionName) const;
	bool KeyExists(SectionData* _sectionData, cstring _keyName) const;

	SectionData* GetSectionData(const fsDataType& _data, cstring _sectionName) const;

	bool TryInsertSection(fsDataType& _dest, cstring _sectionName, SectionData* _sectionData);
	bool FillSectionDataWithAnotherSection(fsDataType& _data, SectionData* _destination, cstring _parentSectionName);

private:
	//string fileName;
	File file;
	//string fileDirectory;
	bool debugMode;
	fsDataType data;
};

template OW_CORE_DLL_API bool FileSystem::Assign(string& T, cstring _sectionName, cstring _keyName);
template OW_CORE_DLL_API bool FileSystem::Assign(bool& T, cstring _sectionName, cstring _keyName);
template OW_CORE_DLL_API bool FileSystem::Assign(double& T, cstring _sectionName, cstring _keyName);
template OW_CORE_DLL_API bool FileSystem::Assign(int& T, cstring _sectionName, cstring _keyName);

#define _FileSystem FileSystem::instance()
