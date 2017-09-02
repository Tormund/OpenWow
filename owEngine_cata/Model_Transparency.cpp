#include "stdafx.h"

// General
#include "Model_Transparency.h"

void ModelTransparency::init(File& f, ModelTransDef& mcd, uint32_t * global)
{
	trans.init(mcd.trans, f, global);
}