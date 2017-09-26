#include "stdafx.h"

#include "MapTilePass.h"

MapTilePass::MapTilePass() : Technique("shaders/mapTilePass.glsl") {}

bool MapTilePass::Init()
{
	if (!CompileProgram("MapTilePass"))
	{
		return false;
	}

	return true;
}

void MapTilePass::SetWVP(cmat4 WVP)
{
	setMat4("gWVP", WVP);
}

void MapTilePass::SetWorldMatrix(cmat4 WorldInverse)
{
	setMat4("gWorld", WorldInverse);
}

void MapTilePass::SetLayersCount(int _layersCount)
{
	setInt("gLayersCount", _layersCount);
}

// ---------------------------------------------------


void MapTilePass::SetColorTextureUnit0(int TextureUnit)
{
	setInt("gColorMap0", TextureUnit);
}

void MapTilePass::SetColorTextureUnit1(int TextureUnit)
{
	setInt("gColorMap1", TextureUnit);
}

void MapTilePass::SetColorTextureUnit2(int TextureUnit)
{
	setInt("gColorMap2", TextureUnit);
}

void MapTilePass::SetColorTextureUnit3(int TextureUnit)
{
	setInt("gColorMap3", TextureUnit);
}

// ----------------------------------------------------

void MapTilePass::SetBlendBuffer(int TextureUnit)
{
	setInt("gBlend", TextureUnit);
}

// ----------------------------------------------------


void MapTilePass::SetShadowMapExists(bool _exists)
{
	setBool("gShadowMapExists", _exists);
}

void MapTilePass::SetShadowColor(vec3 _shadowColor)
{
	setVec3("gShadowColor", _shadowColor);
}

// ----------------------------------------------------

void MapTilePass::SetSpecularTextureUnit(int TextureUnit)
{
	setInt("gSpecularMap", TextureUnit);
}

void MapTilePass::SetMCCVExists(bool _exists)
{
	setBool("gMCCVExists", _exists);
}

void MapTilePass::SetMCLVExists(bool _exists)
{
	setBool("gMCLVExists", _exists);
}