#pragma once

#include "Sky.h"

class MapSkies
{
public:
	MapSkies(DBC_MapRecord* _mapRecord);
	~MapSkies();

	void InitBuffer();

public:
    Sky* m_TempSky;
    Sky* m_CurrentSky;


	vec3 colorSet[COLORS_PARAMS_COUNT];
    float fogSet[FOGS_PARAMS_COUNT];
    float glow;




	void CalculateSkiesWeights(cvec3 pos);
	void initSky(cvec3 pos, uint32 t);
    bool hasSkies() { return !skies.empty(); }

	bool drawSky(cvec3 pos);
    bool DEBUG_Render();


private:
	uint32 __vb;
	uint32 __geom;
	uint32 __vertsSize;

	vector<Sky*> skies;
	//MDX* stars;  // BOUZI FIXME ENABLE ME
};