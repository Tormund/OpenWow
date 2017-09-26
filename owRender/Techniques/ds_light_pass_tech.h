#pragma once

#include "technique.h"
#include "../lights_common.h"

class DSLightPassTech : public Technique
{
public:
	 DSLightPassTech();
	 virtual bool Init();

	 void SetEyeWorldPos(const vec3& EyeWorldPos);
	 void SetMatSpecularIntensity(float Intensity);
	 void SetMatSpecularPower(float Power);
	 void SetScreenSize(unsigned int Width, unsigned int Height);
};