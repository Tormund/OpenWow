#pragma once

#include "../shared/pack_begin.h"

template<typename T>
struct M2Array
{
	uint32_t size;
	uint32_t offset; // pointer to T, relative to begin of m2 data block (i.e. MD21 chunk content or begin of file)
};

struct M2TrackBase
{
	int16_t interpolation_type;
	int16_t global_sequence;
	M2Array<M2Array<uint32_t>> timestamps;
};

template<typename T>
struct M2Track
{
	int16_t interpolation_type;
	int16_t global_sequence;
	M2Array<M2Array<uint32_t>> timestamps;
	M2Array<M2Array<T>> values;
};

template<typename T>
struct M2TrackFake
{
	M2Array<M2Array<uint32_t>> timestamps;
	M2Array<M2Array<T>> values;
};

#include "../shared/pack_end.h"