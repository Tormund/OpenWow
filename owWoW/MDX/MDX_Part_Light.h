#pragma once

enum ModelLightTypes
{
	MODELLIGHT_DIRECTIONAL = 0,
	MODELLIGHT_POINT = 1
};

#include "MDX_Headers.h"

class MDX_Part_Light
{
public:
	void init(File& f, M2Light& mld, uint32* global);
	void setup(int time, uint32 l);

private:
	int type, parent;
	vec3 pos, tpos, dir, tdir;
	Animated<vec3> diffColor, ambColor;
	Animated<float> diffIntensity, ambIntensity;

	//

	friend class MDX;
};