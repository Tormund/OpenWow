#include "stdafx.h"

// General
#include "MDX_Part_Color.h"

void MDX_Part_Color::init(File& f, M2Color& mcd, uint32_t* global)
{
	color.init(mcd.color, f, global);
	opacity.init(mcd.alpha, f, global);
}