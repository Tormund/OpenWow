#pragma once

#include "Map.h"

const int stripsize = 8 * 18 + 7 * 2;
const int stripsize2 = 16 * 18 + 7 * 2 + 8 * 2;

class MapsManager
{
	CLASS_INSTANCE(MapsManager);

	MapsManager();
	~MapsManager();

	void CreateTextureBuffers();
	void ActivateTextureBuffers();
public:
	Map* GetMap() { return &m_map; }

private:
	Map m_map;

public:
	GLuint detailtexcoords, alphatexcoords;
	vec2 dataDetail[C_MapBufferSize];
	vec2 dataAlpha[C_MapBufferSize];
	short* mapstrip;
	short* mapstrip2;
};

#define _MapsManager MapsManager::instance()

