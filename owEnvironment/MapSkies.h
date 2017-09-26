#pragma once

#include "Sky.h"

class MapSkies
{
public:
	MapSkies(uint32_t mapid);
	~MapSkies();

public:
	vec3 colorSet[18];

	void CalculateSkiesWeights(cvec3 pos);
	void initSky(cvec3 pos, int t);

	bool drawSky(cvec3 pos);
	bool hasSkies() { return !skies.empty(); }

private:
	vector<Sky*> skies;
	/*Model* stars;*/  // BOUZI FIXME ENABLE ME
};