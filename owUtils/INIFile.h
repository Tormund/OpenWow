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

class INIFile
{
public:
	 bool Open(cstring filename);
	 void Clear();

	 vector<SectionStruct> GetSections(string _sectionName);
	 SectionStruct* GetFirstSection(string _sectionName);

	template<typename T>
	bool Assign(T& addr, string _keyName);

	template<typename T>
	bool Assign(T& addr, string _section, string _keyName);

	 vector<SectionStruct> Data() const { return data; }

private:
	string iniFilename;
	ifstream iniStream;

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

	Modules::log().Warn("INIFile[%s]: Can't load key[%s] in section[%s]", iniFilename.c_str(), _keyName.c_str(), _section.c_str());

	return false;
}
