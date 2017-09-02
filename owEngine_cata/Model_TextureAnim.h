#pragma once

#include "modelheaders.h"
#include "animated.h"

class ModelTextureAnim
{
public:
	void init(File& f, ModelTexAnimDef& mta, uint32_t * global);
	void calc(int anim, int time);
	void setup(int anim);

public:
	vec3 tval, rval, sval;

private:
	Animated<vec3> trans, rot, scale;
};