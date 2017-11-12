#include "stdafx.h"

// General
#include "Utils.h"

string Utils::ParseSectionName(string& s)
{
	size_t bracket = s.find_first_of(']');
	if (bracket == string::npos)
		return "";

	return s.substr(1, bracket - 1);
}

string Utils::ParseSectionAndIncludeName(string& s, string& _includeSectionName)
{
	size_t bracket = s.find_first_of(']');
	if (bracket == string::npos)
		return "";

	size_t includeSymbol = s.find_first_of(':');
	if (includeSymbol == string::npos)
		_includeSectionName = "";
	else
		_includeSectionName = s.substr(includeSymbol + 1);

	return s.substr(1, bracket - 1);
}

void Utils::ParseKeyPair(string& s, string& key, string& val)
{
	size_t separator = s.find_first_of('=');
	if (separator == string::npos)
	{
		key = "";
		val = "";
		return;
	}

	key = s.substr(0, separator);
	val = s.substr(separator + 1, s.length());

	key = Trim(key);
	val = Trim(val);
}

bool Utils::TryParse(const type_info& type, cstring value, void* output)
{
	stringstream stream(value);

	if (type == typeid(bool))
		stream >> (bool&)*((bool*)output);

	else if (type == typeid(int))
		stream >> (int&)*((int*)output);

	else if (type == typeid(unsigned int))
		stream >> (unsigned int&)*((unsigned int*)output);

	else if (type == typeid(short))
		stream >> (short&)*((short*)output);

	else if (type == typeid(unsigned short))
		stream >> (unsigned short&)*((unsigned short*)output);

	else if (type == typeid(char))
		stream >> (char&)*((char*)output);

	else if (type == typeid(unsigned char))
		stream >> (unsigned char&)*((unsigned char*)output);

	else if (type == typeid(float))
		stream >> (float&)*((float*)output);

	else if (type == typeid(double))
		stream >> (double&)*((double*)output);

	else if (type == typeid(string))
		*((string *)output) = value;

	else
	{
		//Log::Error("UtilsParsing: a required type is not defined! [%s]", value.c_str());
		return false;
	}

	return !stream.fail();
}

// String

string Utils::Trim(string& s, cstring delimiters)
{
	return TrimLeft(TrimRight(s, delimiters), delimiters);
}

string Utils::TrimLeft(string& s, cstring delimiters)
{
	return s.erase(0, s.find_first_not_of(delimiters));
}

string Utils::TrimRight(string& s, cstring delimiters)
{
	return s.erase(s.find_last_not_of(delimiters) + 1);
}

void Utils::ToLower(string& _string)
{
	_string = Trim(_string);
	transform(_string.begin(), _string.end(), _string.begin(), ::tolower);
}

string Utils::ToLower(cstring _string)
{
	string str = _string;
	str = Trim(str);
	transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}


int Utils::popFirstInt(string& s, char separator)
{
	return ToType<int>(GetFirstSubString(s, separator));
}

double Utils::popFirstDouble(string& s, char separator)
{
	return ToType<double>(GetFirstSubString(s, separator));
}

float Utils::popFirstFloat(string& s, char separator)
{
	return ToType<float>(GetFirstSubString(s, separator));
}

string Utils::GetFirstSubString(string& s, char separator)
{
	string outs = "";
	size_t seppos;

	if (separator == 0)
	{
		seppos = s.find_first_of(',');
		size_t alt_seppos = s.find_first_of(';');

		if (alt_seppos != string::npos && alt_seppos < seppos)
		{
			seppos = alt_seppos; // return the first ',' or ';'
		}
	}
	else
	{
		seppos = s.find_first_of(separator);
	}

	if (seppos == string::npos)
	{
		outs = s;
		s = "";
	}
	else
	{
		outs = s.substr(0, seppos);
		s = s.substr(seppos + 1, s.length());
	}

	return outs;
}

string Utils::getNextToken(cstring s, size_t &cursor, char separator)
{
	size_t seppos = s.find_first_of(separator, cursor);
	if (seppos == string::npos)
	{ // not found
		cursor = string::npos;
		return "";
	}
	string outs = s.substr(cursor, seppos - cursor);
	cursor = seppos + 1;
	return outs;
}

string Utils::stripCarriageReturn(cstring line)
{
	if (line.length() > 0)
	{
		if ('\r' == line.at(line.length() - 1))
		{
			return line.substr(0, line.length() - 1);
		}
	}
	return line;
}

string Utils::getLine(ifstream& infile)
{
	string line;
	// This is the standard way to check whether a read failed.
	if (!getline(infile, line))
		return "";
	line = stripCarriageReturn(line);
	return line;
}

bool Utils::PointInRectangle(const vec2 _point, const vec2 _rectStart, const vec2 _rectEnd)
{
	return
		_point.x > _rectStart.x && _point.y > _rectStart.y &&
		_point.x < _rectEnd.x && _point.y < _rectEnd.y;
}
