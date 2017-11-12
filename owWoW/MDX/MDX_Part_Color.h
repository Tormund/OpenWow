#pragma once

#include "MDX_Headers.h"

class MDX_Part_Color
{
public:
	void init(File& f, M2Color& mcd, uint32* global);
	void calc(int anim, int time);

	vec4 getValue()
	{
		return vec4(vColor, vAlpha);
	}

	vec3 getValueColor()
	{
		return vColor;
	}

	float getValueAlpha()
	{
		return vAlpha;
	}

private:
	Animated<vec3> color;
	Animated<float, short, ShortToFloat> alpha;

	vec3 vColor;
	float vAlpha;

	//

	friend class MDX;
	friend class MDX_Skin_Batch;
};