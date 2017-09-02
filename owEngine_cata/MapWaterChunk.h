#pragma once

struct SWaterLayer {
	uint16_t flags;
	uint16_t type;
	float levels[2];
	uint8_t x;
	uint8_t y;
	uint8_t w;
	uint8_t h;

	bool hasmask;
	uint8_t mask[8];

	vector<float> heights;
	vector<uint8_t> depths;
	vector<bool> renderTiles;
};

#include "MapChunk.h"

class MapWaterChunk : public MapChunk {
public:
	MapWaterChunk() : MapChunk() {
		initTextures("XTextures\\river\\lake_a", 1, 30);
	}

	void initTextures(const char *basename, int first, int last);
	void drawWater();

	vector<Texture*> wTextures;
	vector<SWaterLayer> waterLayer;
};