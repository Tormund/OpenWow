#include "stdafx.h"

// General
#include "FileSystem.h"

// Additional

bool FileSystem::Init()
{
	debugMode = true;

	if (!Open("gamedata/system.txt", data, debugMode))
	{
		Modules::log().Error("FileSystem: Can't init filesystem.");
		Destroy();
		return false;
	}

	if (debugMode)
		Print();

	return true;
}

void FileSystem::Print()
{
	Modules::log().Print("FileSystem: BEGIN");

	for (auto sectIt = data.begin(); sectIt != data.end(); ++sectIt)
	{
		Modules::log().Info("");
		Modules::log().Info("[%s]", (*sectIt).first.c_str());

		for (auto keyIt = (*sectIt).second->data.begin(); keyIt != (*sectIt).second->data.end(); ++keyIt)
			Modules::log().Info("%s=%s", (*keyIt).first.c_str(), (*keyIt).second.c_str());
	}

	Modules::log().Print("FileSystem: END");
}

void FileSystem::Destroy()
{
	int deletedSections = 0;
	for (auto sectIt = data.begin(); sectIt != data.end(); ++sectIt)
	{
		(*sectIt).second->data.clear();
		delete (*sectIt).second;
		deletedSections++;
	}
	data.clear();
	Modules::log().Info("FileSystem: Cleaned. Deleted [%d] sections.", deletedSections);
}

// Public functions

string FileSystem::Get(cstring _sectionName, cstring _keyName) const
{
	SectionData* _sectionData = GetSectionData(data, _sectionName);
	if (_sectionData == nullptr)
		return "";

	if (!KeyExists(_sectionData, _keyName))
		return "";

	return _sectionData->data[_keyName];
}

bool FileSystem::IsKeyExists(cstring _sectionName, cstring _keyName)  const
{
	SectionData* _sectionData = GetSectionData(data, _sectionName);
	if (_sectionData == nullptr)
		return false;

	return KeyExists(_sectionData, _keyName);
}

template<typename T>
bool FileSystem::Assign(T& addr, cstring _sectionName, cstring _keyName)
{
	string value = Get(_sectionName, _keyName);
	if (value.empty())
		return false;

	if (!Utils::TryParse(typeid(addr), value, (void*)&addr))
	{
		Modules::log().Error("FS: Can't assign key [%s] to variable in section [%s]", _keyName.c_str(), _sectionName.c_str());
		return false;
	}

	return true;
}

#pragma region Private functional

bool FileSystem::Open(const File& _file, fsDataType& _currentData, bool _debugMode)
{
	file = _file;

	if (!file.Open())
	{
		Modules::log().Error("FileSystem: Can not open file [%s]!", file.Path_Name().c_str());
		return false;
	}

	debugMode = _debugMode;

	SectionData* _currentSection = nullptr;
	string currentSectionName = "";

	while (!file.IsEof())
	{
		string line = file.ReadLine();

		// Skip empty lines
		if (line.length() == 0)
			continue;

		// Find directive
		if (line.at(0) == '!')
		{
			string directiveLine = line.substr(1);

			size_t firstSpacePosition = directiveLine.find(' ');
			if (firstSpacePosition != string::npos)
			{
				string directive = directiveLine.substr(0, firstSpacePosition);

				// Directive Include
				if (directive == "include")
				{
					string inludeFileName = directiveLine.substr(firstSpacePosition + 1);
					Modules::log().Error("Include %s", inludeFileName.c_str());
					FileSystem newFile;
					if (!newFile.Open(File(inludeFileName, file.Path()), _currentData, debugMode))
					{
						Modules::log().Error("FileSystem: Error while loading data from [%s].", inludeFileName.c_str());
						//file.Destroy();
						return false;
					}

					continue;
				}
			}
		}

		// Skip comments
		if (line.at(0) == '#')
			continue;

		if (line.at(0) == '[')
		{
			// Add section
			if (!TryInsertSection(_currentData, currentSectionName, _currentSection))
			{
				//file.Destroy();
				return false;
			}

			// Prepare data for new section
			_currentSection = nullptr;
			currentSectionName = "";

			// Parse new section name
			string parentSectionName;
			currentSectionName = Utils::ParseSectionAndIncludeName(line, parentSectionName);

			// Section name not empty
			if (!currentSectionName.empty())
			{
				_currentSection = new SectionData;

				// We have parent section
				if (!parentSectionName.empty())
				{
					//
					// Try fill section with parent
					if (!FillSectionDataWithAnotherSection(_currentData, _currentSection, parentSectionName))
					{
						Modules::log().Error("FileSystem: Error while fill section [%s] with parent section [%s]. File [%s].", currentSectionName.c_str(), parentSectionName.c_str(), file.Path_Name().c_str());
						return false;
					}
				}
			}

			continue;
		}

		// Add key=value
		string keyName = "";
		string value = "";
		auto equalCharPos = line.find_first_of('=');
		if (equalCharPos != string::npos)
		{
			keyName = Utils::Trim(line.substr(0, equalCharPos));
			value = Utils::Trim(line.substr(equalCharPos + 1, line.length()));
		}

		if (KeyExists(_currentSection, keyName))
		{
			if (debugMode)
			{
				Modules::log().Warn("FileSystem: Dublicate key [%s] in section [%s]. File [%s].", keyName.c_str(), currentSectionName.c_str(), file.Path_Name().c_str());
				Modules::log().Warn("FileSystem: Old value [%s] ", _currentSection->data[keyName].c_str());
				Modules::log().Warn("FileSystem: New value [%s] ", value.c_str());
				Modules::log().Warn("FileSystem: Set new value.", value.c_str());
			}
			_currentSection->data[keyName] = value;
		}
		else
		{
			_currentSection->data.insert(make_pair(keyName, value));
		}
	}

	if (!TryInsertSection(_currentData, currentSectionName, _currentSection))
	{
		//file.Destroy();
		return false;
	}

	//file.Destroy();
	return true;
}

bool FileSystem::SectionExists(const fsDataType& _data, cstring _sectionName)  const
{
	return GetSectionData(_data, _sectionName) != nullptr;
}

bool FileSystem::KeyExists(SectionData* _sectionData, cstring _keyName)  const
{
	if (_sectionData == nullptr)
		return false;

	for (auto keyIt = _sectionData->data.begin(); keyIt != _sectionData->data.end(); ++keyIt)
		if ((*keyIt).first == _keyName)
			return true;

	return false;
}

FileSystem::SectionData* FileSystem::GetSectionData(const fsDataType& _data, cstring _sectionName) const
{
	for (auto it : _data)
		if (it.first == _sectionName)
			return it.second;

	return nullptr;
}

bool FileSystem::FillSectionDataWithAnotherSection(fsDataType& _data, SectionData* _destination, cstring _parentSectionName)
{
	SectionData* _parentSectionData = GetSectionData(_data, _parentSectionName);
	if (_parentSectionData == nullptr)
	{
		Modules::log().Error("FileSystem: Parent section [%s] not exists. File [%s].", _parentSectionName.c_str(), file.Path_Name().c_str());
		return false;
	}

	if (_parentSectionData->data.size() == 0)
	{
		Modules::log().Warn("FileSystem: Parent section [%s] has no data. File [%s].", _parentSectionName.c_str(), file.Path_Name().c_str());
		return true;
	}

	_destination->data.insert(_parentSectionData->data.begin(), _parentSectionData->data.end());

	return true;
}

bool FileSystem::TryInsertSection(fsDataType& _dest, cstring _sectionName, SectionData* _sectionData)
{
	if (_sectionData == nullptr)
		return true;

	if (_sectionName.size() == 0)
	{
		Modules::log().Error("FileSystem: Section [%s] name is empty. File [%s].", _sectionName.c_str(), file.Path_Name().c_str());
		return false;
	}

	if (_sectionData->data.size() == 0)
	{
		Modules::log().Error("FileSystem: Section [%s] data is empty. File [%s].", _sectionName.c_str(), file.Path_Name().c_str());
		return false;
	}

	if (SectionExists(_dest, _sectionName))
	{
		Modules::log().Error("FileSystem: Dublicate section [%s]. File [%s].", _sectionName.c_str(), file.Path_Name().c_str());
		return false;
	}

	_dest.insert(make_pair(_sectionName, _sectionData));

	if (debugMode)
		Modules::log().Info("FileSystem: Section [%s] with size [%d] added.", _sectionName.c_str(), _sectionData->data.size());

	return true;
}

#pragma endregion
