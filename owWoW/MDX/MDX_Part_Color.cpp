#include "../stdafx.h"

// General
#include "MDX_Part_Color.h"

void MDX_Part_Color::init(File& f, M2Color& mcd, uint32* global)
{
	color.init(mcd.color, f, global);
	alpha.init(mcd.alpha, f, global);

	vColor = vec3(1.0f, 1.0f, 1.0f);
	vAlpha = 1.0f;
}

void MDX_Part_Color::calc(int anim, int time)
{
	if (color.uses(anim))
	{
		vColor = color.getValue(anim, time);
		vAlpha = alpha.getValue(anim, time);
	}
}
