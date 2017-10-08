#include "../stdafx.h"

#include "Water_Pass.h"

Water_Pass::Water_Pass() : Technique("shaders/Water_Pass.glsl") {}

bool Water_Pass::Init()
{
	if (!CompileProgram("Water_Pass"))
	{
		return false;
	}

	return true;
}
