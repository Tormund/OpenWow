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
		for (auto it = data.begin(); it != data.end(); ++it)
			if ((*it).key == _keyName)
				return (*it).value;

		return "";
	}
};

class INIFile
{
public:
	OW_CORE_DLL_API bool Open(cstring filename);
	OW_CORE_DLL_API void Clear();

	OW_CORE_DLL_API vector<SectionStruct> GetSections(string _sectionName);
	OW_CORE_DLL_API SectionStruct& GetFirstSection(string _sectionName);

	template<typename T>
	OW_CORE_DLL_API bool Assign(T& addr, string _keyName);

	template<typename T>
	OW_CORE_DLL_API bool Assign(T& addr, string _section, string _keyName);

	OW_CORE_DLL_API vector<SectionStruct> Data() const { return data; }

private:
	string iniFilename;
	ifstream iniStream;

	vector<SectionStruct> data;
};
