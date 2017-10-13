#include "../stdafx.h"

#include "MDX_GeometryPass.h"

MDX_GeometryPass::MDX_GeometryPass() : Technique("shaders/MDX") {}

bool MDX_GeometryPass::Init()
{
	return true;
}