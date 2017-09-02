#pragma once

#include <cstdint>

struct C2Vector
{
	float x;
	float y;
};

struct C3Vector
{
	float x;
	float y;
	float z;
};

struct C3iVector
{
	int x;
	int y;
	int z;
};

struct C4Vector
{
	float x;
	float y;
	float z;
	float w;
};

struct C4iVector
{
	int x;
	int y;
	int z;
	int w;
};

struct C33Matrix
{
	C3Vector columns[3];
};

struct C44Matrix // todo: row or column?
{
	C4Vector columns[4];
};

struct C4Plane
{
	C3Vector normal;
	float distance;
};

struct C4Quaternion
{
	float x;
	float y;
	float z;
	float w; // Unlike Quaternions elsewhere, the scalar part ('w') is the last element in the struct instead of the first
};

struct Range
{
	float min;
	float max;
};

struct CAaBox
{
	C3Vector min;
	C3Vector max;
};

struct CAaSphere
{
	C3Vector position;
	float radius;
};

struct CArgb
{
	CArgb() : r(0), g(0), b(0), a(0) {}
	CArgb(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a) : r(_r), g(_g), b(_b), a(_a) {}

	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};

struct CImVector
{
	CImVector() : b(0), g(0), r(0), a(0) {}
	CImVector(uint8_t _b, uint8_t _g, uint8_t _r, uint8_t _a) : b(_b), g(_g), r(_r), a(_a) {}

	uint8_t b;
	uint8_t g;
	uint8_t r;
	uint8_t a;
};

struct C3sVector
{
	int16_t x;
	int16_t y;
	int16_t z;
};

struct C3Segment
{
	C3Vector start;
	C3Vector end;
};

struct CFacet
{
	C4Plane plane;
	C3Vector vertices[3];
};

struct C3Ray
{
	C3Vector origin;
	C3Vector dir;
};