#include "stdafx.h"

// General
#include "INIFile.h"

bool INIFile::Open(cstring _filename)
{
	iniFilename = _filename;

	if (_filename.empty())
	{
		Modules::log().Error("INIFile[%s]: No such file or directory!", _filename.c_str());
		return false;
	}

	iniStream.open(iniFilename.c_str(), ios::in);
	if (!iniStream.is_open())
	{
		Modules::log().Error("INIFile[%s]: Can not open file!", _filename.c_str());
		return false;
	}

	SectionStruct newSection;
	newSection.section = "root";

	while (iniStream.good())
	{
		string line = Utils::Trim(Utils::getLine(iniStream));

		if (line.length() == 0)
			continue;

		if (line.at(0) == '#')
		{
			continue;
		}
		else if (line.at(0) == '[')
		{
			data.push_back(newSection);
			newSection.data.clear();
			newSection.section = Utils::ParseSectionName(line);
			continue;
		}

		KeyValueStruct keyValueStruct;
		Utils::ParseKeyPair(line, keyValueStruct.key, keyValueStruct.value);
		newSection.data.push_back(keyValueStruct);
	}

	data.push_back(newSection);

	iniStream.close();
	iniStream.clear();

	return true;
}

void INIFile::Clear()
{
	for (auto it : data)
	{
		for (auto it2 : it.data)
		{
			it2.key.clear();
			it2.value.clear();
		}
		it.data.clear();
	}
	data.clear();
}

vector<SectionStruct> INIFile::GetSections(string _sectionName)
{
	vector<SectionStruct> myMap;

	for (auto it : data)
		if (_sectionName == it.section)
			myMap.push_back(it);

	return myMap;
}

SectionStruct* INIFile::GetFirstSection(string _sectionName)
{
	for (auto it : data)
		if (_sectionName == it.section)
			return &it;

	return nullptr;
}
