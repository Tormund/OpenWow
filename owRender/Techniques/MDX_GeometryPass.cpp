#include "../stdafx.h"

#include "MDX_GeometryPass.h"

MDX_GeometryPass::MDX_GeometryPass() : Technique("shaders/MDX_GeometryPass.glsl") {}

bool MDX_GeometryPass::Init()
{
	if (!CompileProgram("MDX_GeometryPass"))
	{
		return false;
	}

	return true;
}