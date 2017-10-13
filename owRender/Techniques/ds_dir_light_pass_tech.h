#pragma once

#include "ds_light_pass_tech.h"
#include "../lights_common.h"

class DSDirLightPassTech : public Technique
{
public:
	DSDirLightPassTech();
	virtual bool Init();

	void SetDirectionalLight(const DirectionalLight& Light);


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