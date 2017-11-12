#include "stdafx.h"

// General
#include "INIFile.h"

bool INIFile::Open()
{
	SectionStruct newSection;
	newSection.section = "root";

	while (!ByteBuffer::IsEof())
	{
		string line = ByteBuffer::ReadLine();

        if (line.length() == 0)
        {
            continue;
        }

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

    ByteBuffer::Clear();

	return true;
}

vector<SectionStruct> INIFile::GetSections(string _sectionName)
{
	vector<SectionStruct> myMap;

    for (auto it : data)
    {
        if (_sectionName == it.section)
        {
            myMap.push_back(it);
        }
    }

	return myMap;
}

SectionStruct* INIFile::GetFirstSection(string _sectionName)
{
	for (auto it : data)
		if (_sectionName == it.section)
			return &it;

	return nullptr;
}
