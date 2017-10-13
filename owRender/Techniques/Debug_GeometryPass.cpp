#include "../stdafx.h"

#include "Debug_GeometryPass.h"

Debug_GeometryPass::Debug_GeometryPass() : Technique("shaders/Debug") {}

bool Debug_GeometryPass::Init()
{
	return true;
}
