#pragma once

#include "modelheaders.h"
#include "animated.h"

struct ModelTransparency
{
public:
	void init(File& f, ModelTransDef& mtd, uint32_t * global);

public:
	AnimatedShort trans;
};