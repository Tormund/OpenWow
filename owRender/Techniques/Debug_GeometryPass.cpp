#include "../stdafx.h"

#include "Debug_GeometryPass.h"

Debug_GeometryPass::Debug_GeometryPass() : Technique("shaders/Debug_GeometryPass.glsl") {}

bool Debug_GeometryPass::Init()
{
	if (!CompileProgram("Debug_GeometryPass"))
	{
		return false;
	}

	return true;
}
