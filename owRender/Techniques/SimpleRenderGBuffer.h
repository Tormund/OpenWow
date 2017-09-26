#pragma once

#include "technique.h"
#include "../lights_common.h"

class SimpleRenderGBuffer : public Technique
{
public:
	SimpleRenderGBuffer();
	 virtual bool Init();

	 void SetScreenSize(unsigned int Width, unsigned int Height);
};