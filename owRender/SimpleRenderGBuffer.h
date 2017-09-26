#pragma once

#include "technique.h"
#include "lights_common.h"

class SimpleRenderGBuffer : public Technique
{
public:
	SimpleRenderGBuffer();
	 virtual bool Init();

	 void SetWVP(cmat4 WVP);

	 void SetPositionTextureUnit(unsigned int TextureUnit);
	 void SetColorTextureUnit(unsigned int TextureUnit);
	 void SetNormalTextureUnit(unsigned int TextureUnit);
	 void SetSpecularTextureUnit(unsigned int TextureUnit);

	 void SetScreenSize(unsigned int Width, unsigned int Height);
};