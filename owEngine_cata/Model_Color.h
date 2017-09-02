#pragma once

#include "modelheaders.h"
#include "animated.h"

class ModelColor
{
public:
	void init(File& f, ModelColorDef& mcd, uint32_t * global);

public:
	Animated<vec3> color;
	AnimatedShort opacity;
};