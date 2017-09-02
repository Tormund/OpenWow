#pragma once



#include "maptile.h"
#include "wmo.h"
#include "frustum.h"
#include "DayNightCycle.h"
#include "sky.h"

#include "Map.h"

class gMapDBRecord;

const float detail_size = 8.0f;

class World
{
public:
	static World* instance()
	{
		static World instance;
		return &instance;
	}

public:
	World();
	~World();

	void initDisplay();

	void tick(float dt);
	void draw();

	void outdoorLighting();
	void outdoorLights(bool on);
	void setupFog();

public: // Getters
	Map* GetMap() { return &m_map; }

private:
	inline void CalculateSquareDistances()
	{
		highresdistance2 = highresdistance * highresdistance;
		mapdrawdistance2 = mapdrawdistance * mapdrawdistance;
		modeldrawdistance2 = modeldrawdistance * modeldrawdistance;
		doodaddrawdistance2 = doodaddrawdistance * doodaddrawdistance;
	}

private:
	Map m_map;

public:
	short* mapstrip;
	short* mapstrip2;

public:
	// Distances
	float mapdrawdistance, modeldrawdistance, doodaddrawdistance, highresdistance, culldistance;
	float mapdrawdistance2, modeldrawdistance2, doodaddrawdistance2, highresdistance2, culldistance2;
	float fogdistance;

	// Fog params
	float l_const, l_linear, l_quadratic;

	// Sky, lighting
	bool hadSky;
	MapSkies* skies;

	Frustum frustum;

	// Lighting
	DayNightCycle* dayNightCycle;
	DayNightPhase dayNightPhase;

	//
	float time, animtime;


	bool lighting, drawmodels, drawdoodads, drawterrain, drawwmo, loading, drawhighres, drawfog, drawColors;
	bool uselowlod;
	bool useshaders;

	GLuint detailtexcoords, alphatexcoords;
};

#define _World World::instance()
