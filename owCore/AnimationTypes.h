#pragma once

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

#include "../shared/pack_end.h"