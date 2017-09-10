#include "stdafx.h"

#include "ds_light_pass_tech.h"

DSLightPassTech::DSLightPassTech() : Technique("shaders/light_pass.glsl") 
{}

bool DSLightPassTech::Init()
{
	return true;
}

void DSLightPassTech::SetWVP(cmat4 WVP)
{
	setMat4("gWVP", WVP);
}


void DSLightPassTech::SetPositionTextureUnit(unsigned int TextureUnit)
{
	setInt("gbuffer.gPositionMap", TextureUnit);
}
void DSLightPassTech::SetColorTextureUnit(unsigned int TextureUnit)
{
	setInt("gbuffer.gColorMap", TextureUnit);
}
void DSLightPassTech::SetNormalTextureUnit(unsigned int TextureUnit)
{
	setInt("gbuffer.gNormalMap", TextureUnit);
}
void DSLightPassTech::SetSpecularTextureUnit(unsigned int TextureUnit)
{
	setInt("gbuffer.gSpecularMap", TextureUnit);
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
