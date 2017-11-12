#pragma once

struct KeyValueStruct
{
	string key, value;
};

struct SectionStruct
{
	string section;
	vector<KeyValueStruct> data;

	string GetValue(string _keyName)
	{
		for (auto it : data)
			if (it.key == _keyName)
				return it.value;

		return "";
	}
};

#include "File.h"

class INIFile final : public File
{
public:
    bool Open() override;

	vector<SectionStruct> GetSections(string _sectionName);
	SectionStruct* GetFirstSection(string _sectionName);

	template<typename T>
	bool Assign(T& addr, string _keyName);

	template<typename T>
	bool Assign(T& addr, string _section, string _keyName);

	 vector<SectionStruct> Data() const { return data; }

private:
	vector<SectionStruct> data;
};

template<typename T>
inline bool INIFile::Assign(T& addr, string _keyName)
{
	return Assign(addr, "root", _keyName);
}

template<typename T>
inline bool INIFile::Assign(T& addr, string _section, string _keyName)
{
	auto section = GetFirstSection(_section);
	if (section.data.size() == 0)
		return false;

	for (auto it : section.data)
		if (it.key == _keyName)
			return Utils::TryParse(typeid(addr), it.value, (void*)&addr);

	Log::Warn("INIFile[%s]: Can't load key[%s] in section[%s]", iniFilename.c_str(), _keyName.c_str(), _section.c_str());

	return false;
}
