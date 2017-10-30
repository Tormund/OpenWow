#include "../stdafx.h"

// General
#include "MDX_Part_TextureWeight.h"

void MDX_Part_TextureWeight::init(File& f, M2TextureWeight& mtd, uint32* global)
{
	trans.init(mtd.weight, f, global);
}

void MDX_Part_TextureWeight::calc(int anim, int time)
{
	tVal = 1.0f;

	if (trans.uses(anim))
	{
		tVal = trans.getValue(anim, time);
	}
}