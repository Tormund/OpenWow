#pragma once

#include "Model_Instance.h"



enum load_phases;

class World;
class MapChunk;
class MapWaterChunk;

class MapTile
{
public:
	MapTile(int x0, int z0);
	~MapTile();

	//

	bool Init(cstring _filename);
	bool parse_adt(cstring _filename, load_phases _phase);

	//

	void draw(MapTilePass* pass);
	void drawWater();
	void drawObjects();
	void drawSky();
	void drawModels();

	//

	MapChunk* getChunk(uint32_t x, uint32_t z);

public:
	vector<Texture*> textures;

#ifdef WMO_INCL
	uint32_t wmoCount;
	vector<string> wmoNames;
	vector<WMOInstance*> wmoInstances;
#endif

#ifdef MDX_INCL
	uint32_t mdxCount;
	vector<string> mdxNames;
	vector<ModelInstance*> mdxInstances;
#endif

	int m_IndexX, m_IndexZ;
	float m_GamePositionX, m_GamePositionZ;

	MapWaterChunk* chunks[C_ChunksInTile][C_ChunksInTile];
};

int indexMapBuf(int x, int y);



