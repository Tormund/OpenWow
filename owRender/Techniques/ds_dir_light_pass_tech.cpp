#include "../stdafx.h"

#include "ds_dir_light_pass_tech.h"


DSDirLightPassTech::DSDirLightPassTech() : Technique("shaders/Common_SimpleVertex.vs", "shaders/POST_Light_Direction.fs") {}

bool DSDirLightPassTech::Init()
{
	return true;
}

void DSDirLightPassTech::SetDirectionalLight(const DirectionalLight& Light)
{
	setVec3("gDirectionalLight.Base.ambient", Light.ambient);
	setVec3("gDirectionalLight.Base.diffuse", Light.diffuse);
	setVec3("gDirectionalLight.Base.specular", Light.specular);

	vec3 Direction = Light.Direction;
	Direction = Direction.normalized();
	setVec3("gDirectionalLight.Direction", Direction);
}