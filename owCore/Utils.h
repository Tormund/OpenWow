#pragma once

class Utils
{
public:
	// Convert
	static   string ToString(const type_info& type, void* value);

	// Common types
	static   short ToShort(cstring _string);
	static   unsigned short ToUShort(cstring _string);

	static   int ToInt(cstring _string);
	static   unsigned int ToUInt(cstring _string);

	static   float ToFloat(cstring _string);
	static   double ToDouble(cstring _string);

	static   long ToLong(cstring _string);
	static   unsigned long ToULong(cstring _string);

	static   bool ToBool(cstring _string);

	// My types
	static  vec2 ToPoint(string& _string);
	static  vec3 ToVector3(string& _string);
	static  Rect ToRect(string& _string);
	static  Color ToColorFromName(string& _string);
	static  Color ToColorFromRGB(string& _string);
	static  Color ToColorFromRGBA(string& _string);
	static  TextAlignW ToTextAlignW(string& _string);
	static  TextAlignH ToTextAlignH(string& _string);
	static  InputMode ToInputMode(string& _string);

	// Parse
	static   int ParseDuration(string& _string);
	static   int ParseDirection(string& _string);
	static   string ParseSectionName(string& _string);
	static   string ParseSectionAndIncludeName(string& _string, string& _includeSectionName);
	static   void ParseKeyPair(string& _string, string& key, string& val);

	static   bool TryParse(const type_info& type, cstring _string, void* output);

	// String
	static   string Trim(string& _string, cstring delimiters = " \f\n\r\t\v");
	static   string TrimLeft(string& _string, cstring delimiters = " \f\n\r\t\v");
	static   string TrimRight(string& _string, cstring delimiters = " \f\n\r\t\v");

	static   void ToLower(string& _string);
	static   string ToLower(cstring _string);

	static   int popFirstInt(string& _string, char separator = 0);
	static   double popFirstDouble(string& _string, char separator = 0);
	static   float popFirstFloat(string& _string, char separator = 0);

	static   string GetFirstSubString(string& _string, char separator = 0);
	static   string getNextToken(cstring _string, size_t& cursor, char separator);
	static   string stripCarriageReturn(cstring line);
	static   string getLine(ifstream& infile);

	static   bool PointInRectangle(const vec2 _point, const vec2 _rectStart, const vec2 _rectEnd);
};

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

inline vec3 fromRGB(uint32_t color)
{
	const float r = ((color & 0xFF0000) >> 16) / 255.0f;
	const float g = ((color & 0x00FF00) >> 8) / 255.0f;
	const float b = ((color & 0x0000FF)) / 255.0f;
	return vec3(r, g, b);
}

inline vec4 fromBGRA(uint32_t color)
{
	const float b = ((color & 0xFF000000) >> 24) / 255.0f;
	const float g = ((color & 0x00FF0000) >> 16) / 255.0f;
	const float r = ((color & 0x0000FF00) >> 8 ) / 255.0f;
	const float a = ((color & 0x000000FF)      ) / 255.0f;
	return vec4(r, g, b, a);
}

