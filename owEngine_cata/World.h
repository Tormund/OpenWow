#pragma once

#include "AssimpModel.h"

#include "maptile.h"
#include "wmo.h"
#include "frustum.h"
#include "DayNightCycle.h"
#include "sky.h"

#include "Map.h"
#include "WoWSettings.h"

class gMapDBRecord;

const float detail_size = 8.0f;

class World
{
	CLASS_INSTANCE(World);

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
	Map m_map;

public:
	short* mapstrip;
	short* mapstrip2;

public:
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

	GLuint detailtexcoords, alphatexcoords;

public: // SHADERS
	GBuffer* m_gbuffer;

	DSGeomPassTech* m_DSGeomPassTech;
	DSPointLightPassTech* m_DSPointLightPassTech;
	DSDirLightPassTech* m_DSDirLightPassTech;
	NullTechnique* m_nullTech;
	SimpleColorTechnique* colorPassTech;

	AssimpModel* quad;
	AssimpModel* sphere;
};

#define _World World::instance()
