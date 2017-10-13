#include "../stdafx.h"

#include "Sky_GeometryPass.h"

Sky_GeometryPass::Sky_GeometryPass() : Technique("shaders/Sky") {}

bool Sky_GeometryPass::Init()
{
	return true;
}
