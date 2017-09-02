#pragma once

const float C_TileSize = 533.33333f;
const float C_ChunkSize = C_TileSize / 16.0f;
const float C_UnitSize = C_ChunkSize / 8.0f;
const float C_ZeroPoint = 32.0f * C_TileSize;

const uint8_t C_ChunksInTile = 16;
const uint8_t C_MapBufferSize = 9 * 9 + 8 * 8;

#include "wmo.h"
#include "model.h"
#include "liquid.h"

enum load_phases;

class World;
class MapChunk;
class MapWaterChunk;

class MapTile
{
public:
	vector<Texture*> textures;

	size_t wmoCount;
	vector<string> wmoNames;
	vector<WMOInstance> wmoInstances;

	size_t mdxCount;
	vector<string> mdxNames;
	vector<ModelInstance> mdxInstances;

	int indexX, indexZ;
	bool ok;

	float xbase, zbase;

	MapWaterChunk* chunks[16][16];
	//MapWaterChunk* waterChunks[16][16];

public:
	MapTile(int x0, int z0, cstring filename);
	~MapTile();

	void draw();
	void drawWater();
	void drawObjects();
	void drawSky();
	void drawModels();

	void parse_adt(char*, load_phases);

	// Get chunk for sub offset x,z
	MapChunk* getChunk(uint32_t x, uint32_t z);
};

int indexMapBuf(int x, int y);


// 8x8x2 version with triangle strips, size = 8*18 + 7*2
const int stripsize = 8 * 18 + 7 * 2;
template <class V>
void stripify(V *in, V *out)
{
	for (int row = 0; row < 8; row++)
	{
		V *thisrow = &in[indexMapBuf(0, row * 2)];
		V *nextrow = &in[indexMapBuf(0, (row + 1) * 2)];

		if (row > 0)
			*out++ = thisrow[0];

		for (int col = 0; col < 9; col++)
		{
			*out++ = thisrow[col];
			*out++ = nextrow[col];
		}

		if (row < 7)
			*out++ = nextrow[8];
	}
}

// high res version, size = 16*18 + 7*2 + 8*2
const int stripsize2 = 16 * 18 + 7 * 2 + 8 * 2;
template <class V>
void stripify2(V *in, V *out)
{
	for (int row = 0; row < 8; row++)
	{
		V *thisrow = &in[indexMapBuf(0, row * 2)];
		V *nextrow = &in[indexMapBuf(0, row * 2 + 1)];
		V *overrow = &in[indexMapBuf(0, (row + 1) * 2)];

		if (row > 0) *out++ = thisrow[0];// jump end
		for (int col = 0; col < 8; col++)
		{
			*out++ = thisrow[col];
			*out++ = nextrow[col];
		}
		*out++ = thisrow[8];
		*out++ = overrow[8];
		*out++ = overrow[8];// jump start
		*out++ = thisrow[0];// jump end
		*out++ = thisrow[0];
		for (int col = 0; col < 8; col++)
		{
			*out++ = overrow[col];
			*out++ = nextrow[col];
		}
		if (row < 8) *out++ = overrow[8];
		if (row < 7) *out++ = overrow[8];// jump start
	}
}
