#include "../stdafx.h"

#include "Billboard_GeometryPass.h"

Billboard_GeometryPass::Billboard_GeometryPass() : Technique("shaders/Billboard.glsl") {}

bool Billboard_GeometryPass::Init()
{
	if (!CompileProgram("Billboard"))
	{
		return false;
	}

	return true;
}