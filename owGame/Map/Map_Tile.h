#pragma once

#include "Map_Chunk.h"

#include "Model_Instance.h"
#include "WMO_Instance.h"

enum load_phases;

class MapTile
{
public:
	MapTile(int x0, int z0);
	~MapTile();

	//

	bool Init(cstring _filename);
	bool parse_adt(cstring _filename, load_phases _phase);

	//

	void draw();
	void drawWater();
	void drawObjects();
	void drawSky();
	void drawModels();

	//

	MapChunk* getChunk(uint32 x, uint32 z);

public:
	vector<Texture*> m_DiffuseTextures;
	vector<Texture*> m_SpecularTextures;

#ifdef WMO_INCL
	uint32 wmoCount;
	vector<string> wmoNames;
	vector<WMOInstance*> wmoInstances;
#endif

#ifdef MDX_INCL
	uint32 mdxCount;
	vector<string> mdxNames;
	vector<ModelInstance*> mdxInstances;
#endif

	int m_IndexX, m_IndexZ;
	float m_GamePositionX, m_GamePositionZ;

	MapChunk* chunks[C_ChunksInTile][C_ChunksInTile];
};





