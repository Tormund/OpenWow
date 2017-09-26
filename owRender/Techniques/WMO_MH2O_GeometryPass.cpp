#include "../stdafx.h"

#include "WMO_MH2O_GeometryPass.h"

WMO_MH2O_GeometryPass::WMO_MH2O_GeometryPass() : Technique("shaders/WMO_MH2O_GeometryPass.glsl") {}

bool WMO_MH2O_GeometryPass::Init()
{
	if (!CompileProgram("WMO_MH2O_GeometryPass"))
	{
		return false;
	}

	return true;
}
