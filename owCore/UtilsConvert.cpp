#include "stdafx.h"
#include "Utils.h"

string Utils::ToString(const type_info& type, void* value)
{
	stringstream stream;

	if (type == typeid(bool))
		stream << *((bool*)value);

	else if (type == typeid(int))
		stream << *((int*)value);

	else if (type == typeid(unsigned int))
		stream << *((unsigned int*)value);

	else if (type == typeid(short))
		stream << *((short*)value);

	else if (type == typeid(unsigned short))
		stream << *((unsigned short*)value);

	else if (type == typeid(char))
		stream << *((char*)value);

	else if (type == typeid(unsigned char))
		stream << *((unsigned char*)value);

	else if (type == typeid(float))
		stream << *((float*)value);

	else if (type == typeid(double))
		stream << *((double*)value);

	else if (type == typeid(string))
		return (string&)*((string *)value);

	else
	{
		Debug::Error("UtilsParsing: a required type is not defined!");
		return "";
	}

	return stream.str();
}

// Common types

short Utils::ToShort(cstring _string)
{
	short result;
	if (!(stringstream(_string) >> result))
		result = 0;
	return result;
}

unsigned short Utils::ToUShort(cstring _string)
{
	unsigned short result;
	if (!(stringstream(_string) >> result))
		result = 0;
	return result;
}

int Utils::ToInt(cstring _string)
{
	int result;
	if (!(stringstream(_string) >> result))
		result = 0;
	return result;
}

unsigned int Utils::ToUInt(cstring _string)
{
	unsigned int result;
	if (!(stringstream(_string) >> result))
		result = 0;
	return result;
}

float Utils::ToFloat(cstring _string)
{
	float result;
	if (!(stringstream(_string) >> result))
		result = 0.0f;
	return result;
}

double Utils::ToDouble(cstring _string)
{
	double result;
	if (!(stringstream(_string) >> result))
		result = 0.0;
	return result;
}

long Utils::ToLong(cstring _string)
{
	long result;
	if (!(stringstream(_string) >> result))
		result = 0;
	return result;
}

unsigned long Utils::ToULong(cstring _string)
{
	unsigned long result;
	if (!(stringstream(_string) >> result))
		result = 0;
	return result;
}

bool Utils::ToBool(cstring _string)
{
	string _str = _string;
	Trim(_str);
	transform(_str.begin(), _str.end(), _str.begin(), ::tolower);

	if (_str == "true") return true;
	if (_str == "1") return true;

	if (_str == "false") return false;
	if (_str == "0") return false;

	Debug::Error("Utils: %s %s doesn't know how to handle %s", __FILE__, __FUNCTION__, _string.c_str());
	return false;
}

// My types

vec2 Utils::ToPoint(string& _string)
{
	float _x = popFirstFloat(_string);
	float _y = popFirstFloat(_string);
	return vec2(_x, _y);
}

vec3 Utils::ToVector3(string& _string)
{
	float _x = popFirstFloat(_string);
	float _y = popFirstFloat(_string);
	float _z = popFirstFloat(_string);
	return vec3(_x, _y, _z);
}

Rect Utils::ToRect(string& _string)
{
	float _x = popFirstFloat(_string);
	float _y = popFirstFloat(_string);
	float _w = popFirstFloat(_string);
	float _h = popFirstFloat(_string);
	return Rect(_x, _y, _w, _h);
}

Color Utils::ToColorFromName(string& _string)
{
	ToLower(_string);

	if (_string == "empty")
		return COLOR_EMPTY;

	else if (_string == "black")
		return COLOR_BLACK;

	else if (_string == "darkgray")
		return COLOR_DARKGRAY;

	else if (_string == "gray")
		return COLOR_GRAY;

	else if (_string == "lightgray")
		return COLOR_LIGHTGRAY;

	else if (_string == "white")
		return COLOR_WHITE;

	else if (_string == "red")
		return COLOR_RED;

	else if (_string == "green")
		return COLOR_GREEN;

	else if (_string == "blue")
		return COLOR_BLUE;

	else if (_string == "lightblue")
		return COLOR_LIGHTBLUE;

	else if (_string == "pink")
		return COLOR_PINK;

	else if (_string == "yellow")
		return COLOR_YELLOW;

	else
		return COLOR_EMPTY;
}

Color Utils::ToColorFromRGB(string& _string)
{
	Color c;
	c.red = static_cast<float>(popFirstFloat(_string)) / 255.0f;
	c.green = static_cast<float>(popFirstFloat(_string)) / 255.0f;
	c.blue = static_cast<float>(popFirstFloat(_string)) / 255.0f;
	c.alpha = 1.0;
	return c;
}

Color Utils::ToColorFromRGBA(string& _string)
{
	Color c;
	c.red = static_cast<float>(popFirstFloat(_string)) / 255.0f;
	c.green = static_cast<float>(popFirstFloat(_string)) / 255.0f;
	c.blue = static_cast<float>(popFirstFloat(_string)) / 255.0f;
	c.alpha = static_cast<float>(popFirstInt(_string)) / 255.0f;
	return c;
}

TextAlignW Utils::ToTextAlignW(string& _string)
{
	ToLower(_string);

	if (_string == "left")
		return TextAlignW::TEXT_ALIGNW_LEFT;

	else if (_string == "center")
		return TextAlignW::TEXT_ALIGNW_CENTER;

	else if (_string == "right")
		return TextAlignW::TEXT_ALIGNW_RIGHT;

	else
		return TextAlignW::TEXT_ALIGNW_LEFT;
}

TextAlignH Utils::ToTextAlignH(string& _string)
{
	ToLower(_string);

	if (_string == "top")
		return TextAlignH::TEXT_ALIGNH_TOP;

	else if (_string == "center")
		return TextAlignH::TEXT_ALIGNH_CENTER;

	else if (_string == "bottom")
		return TextAlignH::TEXT_ALIGNH_BOTTOM;

	// Default
	else
		return TextAlignH::TEXT_ALIGNH_BOTTOM;
}

InputMode Utils::ToInputMode(string& _string)
{
	ToLower(_string);

	if (_string == "any")
		return InputMode::MODE_ANY;

	else if (_string == "characters")
		return InputMode::MODE_CHARACTERS;

	else if (_string == "numbers")
		return InputMode::MODE_NUMBERS;

	// Default
	else
		return InputMode::MODE_ANY;
}
