#pragma once

#include "technique.h"
#include "lights_common.h"

class DSLightPassTech : public Technique
{
public:
	 DSLightPassTech();
	 virtual bool Init();

	 void SetWVP(cmat4 WVP);

	 void SetPositionTextureUnit(unsigned int TextureUnit);
	 void SetColorTextureUnit(unsigned int TextureUnit);
	 void SetNormalTextureUnit(unsigned int TextureUnit);
	 void SetSpecularTextureUnit(unsigned int TextureUnit);

	 void SetEyeWorldPos(const vec3& EyeWorldPos);
	 void SetMatSpecularIntensity(float Intensity);
	 void SetMatSpecularPower(float Power);
	 void SetScreenSize(unsigned int Width, unsigned int Height);
};