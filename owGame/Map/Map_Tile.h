#pragma once

#include "Map_Chunk.h"

#include "Model_Instance.h"
#include "WMO_Instance.h"

enum load_phases;

class MapTile
{
public:
	MapTile(uint32 _intexX, uint32 _intexZ);
	~MapTile();

	//

	bool Load(cstring _filename);
	bool Load_SplitFile(cstring _filename, load_phases _phase);

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
	vector<string> m_WMOsNames;
	vector<WMOInstance*> m_WMOsInstances;
#endif

#ifdef MDX_INCL
	vector<string> m_MDXsNames;
	vector<ModelInstance*> m_MDXsInstances;
#endif

	int m_IndexX, m_IndexZ;
	float m_GamePositionX, m_GamePositionZ;

	MapChunk* m_Chunks[C_ChunksInTile][C_ChunksInTile];
};





