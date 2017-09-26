#pragma once

enum ModelLightTypes
{
	MODELLIGHT_DIRECTIONAL = 0,
	MODELLIGHT_POINT = 1
};

class MDX_Part_Light
{
public:
	void init(File& f, M2Light& mld, uint32_t* global);
	void setup(int time, GLuint l);

public:
	int type, parent;
	vec3 pos, tpos, dir, tdir;
	Animated<vec3> diffColor, ambColor;
	Animated<float> diffIntensity, ambIntensity;
};