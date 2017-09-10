#include "stdafx.h"

#include "ds_dir_light_pass_tech.h"


DSDirLightPassTech::DSDirLightPassTech() {}

bool DSDirLightPassTech::Init()
{
	if (!CompileProgram("DirLightPass"))
	{
		return false;
	}

	if (!DSLightPassTech::Init())
	{
		return false;
	}

	return true;
}

void DSDirLightPassTech::SetDirectionalLight(const DirectionalLight& Light)
{
	setVec3("gDirectionalLight.Base.ambient", Light.ambient);
	setVec3("gDirectionalLight.Base.diffuse", Light.diffuse);
	setVec3("gDirectionalLight.Base.specular", Light.specular);

	vec3 Direction = Light.Direction;
	Direction = glm::normalize(Direction);
	setVec3("gDirectionalLight.Direction", Direction);
}