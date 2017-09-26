#include "../stdafx.h"

#include "ds_light_pass_tech.h"

DSLightPassTech::DSLightPassTech() : Technique("shaders/light_pass.glsl") 
{}

bool DSLightPassTech::Init()
{
	return true;
}


void DSLightPassTech::SetEyeWorldPos(const vec3& EyePos)
{
	setVec3("gEyeWorldPos", EyePos);
}


void DSLightPassTech::SetMatSpecularIntensity(float Intensity)
{
	setFloat("gMatSpecularIntensity", Intensity);
}

void DSLightPassTech::SetMatSpecularPower(float Power)
{
	setFloat("gSpecularPower", Power);
}

void DSLightPassTech::SetScreenSize(unsigned int Width, unsigned int Height)
{
	setVec2("gScreenSize", (float)Width, (float)Height);
}
