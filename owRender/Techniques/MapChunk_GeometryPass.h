#pragma once

#include "technique.h"

class MapChunk_GeometryPass : public Technique {
public:
	MapChunk_GeometryPass() : Technique("shaders/MapChunk") {}

	// ---------------------------------------------------

	void SetLayersCount(int _layersCount)
	{
		setInt("gLayersCount", _layersCount);
	}

	void SetColorTextureUnit0(int TextureUnit)
	{
		setTexture("gColorMap0", TextureUnit);
	}

	void SetColorTextureUnit1(int TextureUnit)
	{
		setTexture("gColorMap1", TextureUnit);
	}

	void SetColorTextureUnit2(int TextureUnit)
	{
		setTexture("gColorMap2", TextureUnit);
	}

	void SetColorTextureUnit3(int TextureUnit)
	{
		setTexture("gColorMap3", TextureUnit);
	}

	// ----------------------------------------------------

	void SetBlendBuffer(int TextureUnit)
	{
		setTexture("gBlend", TextureUnit);
	}

	// ----------------------------------------------------


	void SetShadowMapExists(bool _exists)
	{
		setInt("gShadowMapExists", _exists);
	}

	void SetShadowColor(vec3 _shadowColor)
	{
		setVec3("gShadowColor", _shadowColor);
	}

	// ----------------------------------------------------

	void SetSpecularTextureUnit0(int TextureUnit)
	{
		setTexture("gSpecularMap0", TextureUnit);
	}

	void SetSpecularTextureUnit1(int TextureUnit)
	{
		setTexture("gSpecularMap1", TextureUnit);
	}

	void SetSpecularTextureUnit2(int TextureUnit)
	{
		setTexture("gSpecularMap2", TextureUnit);
	}

	void SetSpecularTextureUnit3(int TextureUnit)
	{
		setTexture("gSpecularMap3", TextureUnit);
	}

	void SetMCCVExists(bool _exists)
	{
		setInt("gMCCVExists", _exists);
	}

	void SetMCLVExists(bool _exists)
	{
		setInt("gMCLVExists", _exists);
	}
};