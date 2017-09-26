#pragma once

#include "Map_Chunk_Types.h"

class Liquid;
class MapTile;

class MapChunk
{

public:
	MapChunk(MapTile* _parentTile);
	~MapChunk();

	void init(File& f, load_phases _phase);

	void initStrip(int holes);

	void draw2();


	void drawNoDetail();
	void drawPass(int anim);
	//void drawWater();

public:
	MapTile* m_ParentTile;
	float m_GamePositionX, m_GamePositionY, m_GamePositionZ;
	float r;

	int areaID;

	bool haswater;

	bool visible;
	bool hasholes;
	float waterlevel[2];

	Texture* textures[4];
	Texture* SpecularTextures[4];
	//Texture* alphamaps[3];
	//Texture* shadow;

	Texture* blend;

	int animated[4];



	GLuint globalBuffer;

	short* strip;
	int striplen;

	Liquid* lq;

	vec3 vmin, vmax, vcenter;

private:
	MCNK_Header* header;
	bool MCLV_exists;
	bool colorBufferEnable;


	//
	// MH2O Functional
	//


public:
	void TryInitMH2O(MH2O_Header* _header, File& f);
	void createBuffer();

	void initTextures(const char *basename, int first, int last);
	void drawWater();

	vector<MH2O_WaterLayer> m_WaterLayers;

private:
	GLuint globalBufferWater;
	uint32_t globalBufferSize;	

	vector<Texture*> wTextures;
	
};