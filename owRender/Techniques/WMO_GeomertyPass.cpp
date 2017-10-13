#include "../stdafx.h"

#include "WMO_GeomertyPass.h"

WMO_GeomertyPass::WMO_GeomertyPass() : Technique("shaders/WMO") {}

bool WMO_GeomertyPass::Init()
{
	if (!CompileProgram("WMO_GeomertyPass"))
	{
		return false;
	}

	return true;
}
