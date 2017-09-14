#pragma once

#include "ModelHeaders.h"

class ModelColor
{
public:
	void init(File& f, M2Color& mcd, uint32_t * global)
	{
		color.init(mcd.color, f, global);
		opacity.init(mcd.alpha, f, global);
	}

public:
	Animated<vec3> color;
	Animated<float, short, ShortToFloat> opacity;
};