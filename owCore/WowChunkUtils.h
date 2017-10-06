#pragma once

inline void flipcc(char* fcc)
{
	std::swap(fcc[0], fcc[3]);
	std::swap(fcc[1], fcc[2]);
}

#define WOWCHUNK_READ_STRINGS_BEGIN \
char* __tbuf = new char[size + 1]; \
f.ReadBytes(__tbuf, size); \
__tbuf[size] = 0; \
char* __tp = __tbuf; \
while (__tp < __tbuf + size) \
{ \
	string _string(__tp); \
	__tp += _string.length() + 1; \

#define WOWCHUNK_READ_STRINGS_END \
} \
delete[] __tbuf;


#define WOWCHUNK_READ_STRINGS2_BEGIN \
char* p = (char*)f.GetDataFromCurrent(); \
char* end = p + size; \
int t = 0; \
while (p < end) \
{ \
	string _string(p); \
	p += strlen(p) + 1; \
	while ((p < end) && (*p == 0)) p++;

#define WOWCHUNK_READ_STRINGS2_END \
} \
f.SeekRelative(size);