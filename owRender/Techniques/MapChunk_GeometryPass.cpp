#include "../stdafx.h"

#include "MapChunk_GeometryPass.h"

MapChunk_GeometryPass::MapChunk_GeometryPass() : Technique("shaders/MapChunk") {}

bool MapChunk_GeometryPass::Init()
{
	if (!CompileProgram("MapChunk_GeometryPass"))
	{
		return false;
	}

	return true;
}

void MapChunk_GeometryPass::SetLayersCount(int _layersCount)
{
	setInt("gLayersCount", _layersCount);
}

// ---------------------------------------------------


void MapChunk_GeometryPass::SetColorTextureUnit0(int TextureUnit)
{
	setTexture("gColorMap0", TextureUnit);
}

void MapChunk_GeometryPass::SetColorTextureUnit1(int TextureUnit)
{
	setTexture("gColorMap1", TextureUnit);
}

void MapChunk_GeometryPass::SetColorTextureUnit2(int TextureUnit)
{
	setTexture("gColorMap2", TextureUnit);
}

void MapChunk_GeometryPass::SetColorTextureUnit3(int TextureUnit)
{
	setTexture("gColorMap3", TextureUnit);
}

// ----------------------------------------------------

void MapChunk_GeometryPass::SetBlendBuffer(int TextureUnit)
{
	setTexture("gBlend", TextureUnit);
}

// ----------------------------------------------------


void MapChunk_GeometryPass::SetShadowMapExists(bool _exists)
{
	setBool("gShadowMapExists", _exists);
}

void MapChunk_GeometryPass::SetShadowColor(vec3 _shadowColor)
{
	setVec3("gShadowColor", _shadowColor);
}

// ----------------------------------------------------

void MapChunk_GeometryPass::SetSpecularTextureUnit0(int TextureUnit)
{
	setTexture("gSpecularMap0", TextureUnit);
}

void MapChunk_GeometryPass::SetSpecularTextureUnit1(int TextureUnit)
{
	setTexture("gSpecularMap1", TextureUnit);
}

void MapChunk_GeometryPass::SetSpecularTextureUnit2(int TextureUnit)
{
	setTexture("gSpecularMap2", TextureUnit);
}

void MapChunk_GeometryPass::SetSpecularTextureUnit3(int TextureUnit)
{
	setTexture("gSpecularMap3", TextureUnit);
}

void MapChunk_GeometryPass::SetMCCVExists(bool _exists)
{
	setBool("gMCCVExists", _exists);
}

void MapChunk_GeometryPass::SetMCLVExists(bool _exists)
{
	setBool("gMCLVExists", _exists);
}