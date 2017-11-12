#pragma once

#include "Sky.h"



enum SkyColorNames
{
	LIGHT_GLOBAL_DIFFUSE,
	LIGHT_GLOBAL_AMBIENT,

	SKY_COLOR_0,
	SKY_COLOR_1,
	SKY_COLOR_2,
	SKY_COLOR_3,
	SKY_COLOR_4,

	FOG_COLOR,

	SKY_UNKNOWN_1,

	SUN_COLOR,
	SUN_HALO_COLOR,

	SKY_UNKNOWN_2,

	CLOUD_COLOR,

	SKY_UNKNOWN_3,

	OCEAN_COLOR_LIGHT,
	OCEAN_COLOR_DARK,

	RIVER_COLOR_LIGHT,
	RIVER_COLOR_DARK
};

class MapSkies
{
public:
	MapSkies(DBC_MapRecord* _mapRecord);
	~MapSkies();

	void InitBuffer();

public:
	vec3 colorSet[SKY_COLORSCOUNT];

	void CalculateSkiesWeights(cvec3 pos);
	void initSky(cvec3 pos, uint32 t);
    bool hasSkies() { return !skies.empty(); }

    DBC_LightRecord* GetLightRecordByMap(DBC_MapRecord* _mapRecord);

	bool drawSky(cvec3 pos);
    bool DEBUG_Render();


private:
	uint32 __vb;
	uint32 __geom;
	uint32 __vertsSize;

	vector<vec3> colors;

	vector<Sky*> skies;
	//MDX* stars;  // BOUZI FIXME ENABLE ME
};