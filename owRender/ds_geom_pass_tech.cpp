#include "stdafx.h"

#include "ds_geom_pass_tech.h"

DSGeomPassTech::DSGeomPassTech() : Technique("shaders/geometry_pass.glsl") {}

bool DSGeomPassTech::Init()
{
	if (!CompileProgram("GeometryPass"))
	{
		return false;
	}

	return true;
}

void DSGeomPassTech::SetWVP(cmat4 WVP)
{
	setMat4("gWVP", WVP);
}

void DSGeomPassTech::SetWorldMatrix(cmat4 WorldInverse)
{
	setMat4("gWorld", WorldInverse);
}

void DSGeomPassTech::SetColorTextureUnit(int TextureUnit)
{
	setInt("gColorMap", TextureUnit);
}

void DSGeomPassTech::SetSpecularTextureUnit(int TextureUnit)
{
	setInt("gSpecularMap", TextureUnit);
}