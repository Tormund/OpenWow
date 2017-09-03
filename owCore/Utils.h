#pragma once

class Utils
{
public:
	// Convert
	static  OW_CORE_DLL_API string ToString(const type_info& type, void* value);

	// Common types
	static  OW_CORE_DLL_API short ToShort(cstring _string);
	static  OW_CORE_DLL_API unsigned short ToUShort(cstring _string);

	static OW_CORE_DLL_API  int ToInt(cstring _string);
	static OW_CORE_DLL_API  unsigned int ToUInt(cstring _string);

	static OW_CORE_DLL_API  float ToFloat(cstring _string);
	static OW_CORE_DLL_API  double ToDouble(cstring _string);

	static OW_CORE_DLL_API  long ToLong(cstring _string);
	static OW_CORE_DLL_API  unsigned long ToULong(cstring _string);

	static OW_CORE_DLL_API  bool ToBool(cstring _string);

	// My types
	static OW_CORE_DLL_API vec2 ToPoint(string& _string);
	static OW_CORE_DLL_API vec3 ToVector3i(string& _string);
	static OW_CORE_DLL_API vec3 ToVector3d(string& _string);
	static OW_CORE_DLL_API vec3 ToVector3f(string& _string);
	static OW_CORE_DLL_API Rect<int> ToRect(string& _string);
	static OW_CORE_DLL_API Color ToColorFromName(string& _string);
	static OW_CORE_DLL_API Color ToColorFromRGB(string& _string);
	static OW_CORE_DLL_API Color ToColorFromRGBA(string& _string);
	static OW_CORE_DLL_API TextAlignW ToTextAlignW(string& _string);
	static OW_CORE_DLL_API TextAlignH ToTextAlignH(string& _string);
	static OW_CORE_DLL_API InputMode ToInputMode(string& _string);

	// Parse
	static OW_CORE_DLL_API  int ParseDuration(string& _string);
	static OW_CORE_DLL_API  int ParseDirection(string& _string);
	static OW_CORE_DLL_API  string ParseSectionName(string& _string);
	static OW_CORE_DLL_API  string ParseSectionAndIncludeName(string& _string, string& _includeSectionName);
	static OW_CORE_DLL_API  void ParseKeyPair(string& _string, string& key, string& val);

	static OW_CORE_DLL_API  bool TryParse(const type_info& type, cstring _string, void* output);

	// String
	static OW_CORE_DLL_API  string Trim(string& _string, cstring delimiters = " \f\n\r\t\v");
	static OW_CORE_DLL_API  string TrimLeft(string& _string, cstring delimiters = " \f\n\r\t\v");
	static OW_CORE_DLL_API  string TrimRight(string& _string, cstring delimiters = " \f\n\r\t\v");

	static OW_CORE_DLL_API  void ToLower(string& _string);
	static OW_CORE_DLL_API  string ToLower(cstring _string);

	static OW_CORE_DLL_API  int popFirstInt(string& _string, char separator = 0);
	static OW_CORE_DLL_API  double popFirstDouble(string& _string, char separator = 0);
	static OW_CORE_DLL_API  float popFirstFloat(string& _string, char separator = 0);

	static OW_CORE_DLL_API  string GetFirstSubString(string& _string, char separator = 0);
	static OW_CORE_DLL_API  string getNextToken(cstring _string, size_t& cursor, char separator);
	static OW_CORE_DLL_API  string stripCarriageReturn(cstring line);
	static OW_CORE_DLL_API  string getLine(ifstream& infile);

	static OW_CORE_DLL_API  bool PointInRectangle(const vec2 _point, const vec2 _rectStart, const vec2 _rectEnd);

	// Standarts
	template <typename T>
	static T Min(T& _value, T _min);

	template <typename T>
	static T Max(T& _value, T _max);

	template <typename T>
	static T Clamp(T& _value, T _min, T _max);
};

template <typename T>
T Utils::Min(T& _value, T _min)
{
	return (_value < _min) ? _value : _min;
}

template <typename T>
T Utils::Max(T& _value, T _max)
{
	return (_value > _max) ? _value : _max;
}

template <typename T>
T Utils::Clamp(T& _value, T _min, T _max)
{
	return Max(_min, Min(_value, _max));
}

inline bool getBitH2L(uint8_t* data, uint32_t bit)
{
	uint8_t mask = 0x80;
	mask >>= (bit % 8);

	return ((data[bit / 8]) & mask) == mask;
}

inline bool getBitL2H(uint8_t* data, uint32_t bit)
{
	uint8_t mask = 0x1;
	mask <<= (bit % 8);

	return ((data[bit / 8]) & mask) == mask;
}

inline vec4 fromARGB(uint32_t color)
{
	const float a = ((color & 0xFF000000) >> 24) / 255.0f;
	const float r = ((color & 0x00FF0000) >> 16) / 255.0f;
	const float g = ((color & 0x0000FF00) >> 8 ) / 255.0f;
	const float b = ((color & 0x000000FF)      ) / 255.0f;
	return vec4(r, g, b, a);
}

inline vec4 fromBGRA(uint32_t color)
{
	const float b = ((color & 0xFF000000) >> 24) / 255.0f;
	const float g = ((color & 0x00FF0000) >> 16) / 255.0f;
	const float r = ((color & 0x0000FF00) >> 8 ) / 255.0f;
	const float a = ((color & 0x000000FF)      ) / 255.0f;
	return vec4(r, g, b, a);
}