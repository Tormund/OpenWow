#include "stdafx.h"

// General
#include "Model_Color.h"

void ModelColor::init(File& f, ModelColorDef& mcd, uint32_t * global)
{
	color.init(mcd.color, f, global);
	opacity.init(mcd.opacity, f, global);
}