#pragma once

#include "animated.h"

class ModelLight
{
public:
	void init(File& f, ModelLightDef& mld, uint32_t * global);
	void setup(int time, GLuint l);

public:
	int type, parent;
	vec3 pos, tpos, dir, tdir;
	Animated<vec3> diffColor, ambColor;
	Animated<float> diffIntensity, ambIntensity;
};