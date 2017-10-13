#include "../stdafx.h"

#include "ds_light_pass_tech.h"

DSLightPassTech::DSLightPassTech() : Technique("shaders/light_pass.glsl") 
{}

bool DSLightPassTech::Init()
{
	return true;
}
