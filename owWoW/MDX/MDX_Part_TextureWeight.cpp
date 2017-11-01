#include "../stdafx.h"

// General
#include "MDX_Part_TextureWeight.h"

void MDX_Part_TextureWeight::init(File& f, M2TextureWeight& mtd, uint32* global)
{
	trans.init(mtd.weight, f, global);

	tVal = 1.0f;
}

void MDX_Part_TextureWeight::calc(int anim, int time)
{
	if (trans.uses(anim))
	{
		tVal = trans.getValue(anim, time);
	}
}