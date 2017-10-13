#include "../stdafx.h"

#include "Water_Pass.h"

Water_Pass::Water_Pass() : Technique("shaders/Water") {}

bool Water_Pass::Init()
{
	if (!CompileProgram("Water_Pass"))
	{
		return false;
	}

	return true;
}
