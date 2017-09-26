#include "../stdafx.h"

#include "MapTileLowRes_GeometryPass.h"

MapTileLowRes_GeometryPass::MapTileLowRes_GeometryPass() : Technique("shaders/MapTileLowRes_GeometryPass.glsl") {}

bool MapTileLowRes_GeometryPass::Init()
{
	if (!CompileProgram("MapTileLowRes_GeometryPass"))
	{
		return false;
	}

	return true;
}
