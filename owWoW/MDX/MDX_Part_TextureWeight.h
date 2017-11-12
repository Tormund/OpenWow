#pragma once

#include "MDX_Headers.h"

struct MDX_Part_TextureWeight
{
public:
	void init(File& f, M2TextureWeight& mtd, uint32* global);

	void calc(int anim, int time);
	inline float getValue()
	{
		return tVal;
	}

private:
	Animated<float, short, ShortToFloat> trans;
	float tVal;

	//

	friend class MDX;
	friend class MDX_Skin_Batch;
};