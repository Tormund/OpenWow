#pragma once

#include "modelheaders.h"

struct ModelTransDef
{
	AnimationBlock trans;
};

struct ModelTransparency
{
public:
	void init(File& f, ModelTransDef& mtd, uint32_t* global)
	{
		trans.init(mtd.trans, f, global);
	}

public:
	AnimatedShort trans;
};