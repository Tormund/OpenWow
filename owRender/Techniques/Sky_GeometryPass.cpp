#include "../stdafx.h"

#include "Sky_GeometryPass.h"

Sky_GeometryPass::Sky_GeometryPass() : Technique("shaders/Sky_GeometryPass.glsl") {}

bool Sky_GeometryPass::Init()
{
	if (!CompileProgram("Sky_GeometryPass"))
	{
		return false;
	}

	return true;
}
