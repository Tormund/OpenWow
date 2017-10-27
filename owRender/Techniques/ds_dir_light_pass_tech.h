#pragma once

#include "ds_light_pass_tech.h"
#include "../lights_common.h"

class DSDirLightPassTech : public Technique
{
public:
	DSDirLightPassTech() : Technique("shaders/Common_SimpleVertex.vs", "shaders/POST_Light_Direction.fs") {}

	void SetDirectionalLight(const DirectionalLight& Light)
	{
		setVec3("gDirectionalLight.Base.ambient", Light.ambient);
		setVec3("gDirectionalLight.Base.diffuse", Light.diffuse);
		setVec3("gDirectionalLight.Base.specular", Light.specular);

		vec3 Direction = Light.Direction;
		Direction = Direction.normalized();
		setVec3("gDirectionalLight.Direction", Direction);
	}


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