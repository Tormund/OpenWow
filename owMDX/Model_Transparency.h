#pragma once

#include "ModelHeaders.h"

struct ModelTransparency
{
public:
	void init(File& f, M2TextureWeight& mtd, uint32_t* global)
	{
		trans.init(mtd.weight, f, global);
	}

public:
	Animated<float, short, ShortToFloat> trans;
};