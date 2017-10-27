#pragma once

#include "technique.h"
#include "../lights_common.h"

class SimpleRenderGBuffer : public Technique
{
public:
	SimpleRenderGBuffer() : Technique("shaders/Common_SimpleVertex.vs", "shaders/POST_SimpleRender.fs") {}

	 void SetScreenSize(unsigned int Width, unsigned int Height)
	 {
		 setVec2("gScreenSize", (float)Width, (float)Height);
	 }
};