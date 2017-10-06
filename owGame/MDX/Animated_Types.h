#pragma once

enum Interpolations
{
	INTERPOLATION_NONE,
	INTERPOLATION_LINEAR,
	INTERPOLATION_HERMITE
};

#include "../shared/pack_begin.h"

template<typename T>
struct M2Array
{
	uint32 size;
	uint32 offset; // pointer to T, relative to begin of m2 data block (i.e. MD21 chunk content or begin of file)
};

struct M2TrackBase
{
	int16 interpolation_type;
	int16 global_sequence;
	M2Array<M2Array<uint32>> timestamps;
};

template<typename T>
struct M2Track
{
	int16 interpolation_type;
	int16 global_sequence;
	M2Array<M2Array<uint32>> timestamps;
	M2Array<M2Array<T>> values;
};

template<typename T>
struct M2TrackFake
{
	M2Array<M2Array<uint32>> timestamps;
	M2Array<M2Array<T>> values;
};

struct M2CompQuat
{
	__int16 x, y, z, w;
};

#include "../shared/pack_end.h"

//---------------------------------------------------------//
//--                   Converters                        --//
//---------------------------------------------------------//

template <class T>
class Identity
{
public:
	static const T& conv(const T& t)
	{
		return t;
	}
};

class Quat16ToQuat32
{
public:
	static const Quaternion conv(const M2CompQuat t)
	{
		return Quaternion(
			float(t.x > 0 ? t.x - 32767 : t.x + 32767) / 32767.0f,
			float(t.y > 0 ? t.y - 32767 : t.y + 32767) / 32767.0f,
			float(t.z > 0 ? t.z - 32767 : t.z + 32767) / 32767.0f,
			float(t.w > 0 ? t.w - 32767 : t.w + 32767) / 32767.0f);
	}
};

class ShortToFloat
{
public:
	static const float conv(const short t)
	{
		return t / 32767.0f;
	}
};