#pragma once

#include "technique.h"
#include "../lights_common.h"

class DSLightPassTech : public Technique
{
public:
	DSLightPassTech() : Technique("shaders/light_pass.glsl") {}

	 void SetEyeWorldPos(const vec3& EyePos)
	 {
		 setVec3("gEyeWorldPos", EyePos);
	 }

	 void SetMatSpecularIntensity(float Intensity)
	 {
		 setFloat("gMatSpecularIntensity", Intensity);
	 }

	 void SetMatSpecularPower(float Power)
	 {
		 setFloat("gSpecularPower", Power);
	 }

	 void SetScreenSize(unsigned int Width, unsigned int Height)
	 {
		 setVec2("gScreenSize", (float)Width, (float)Height);
	 }
};