#pragma once

#include "technique.h"

class MDX_GeometryPass : public Technique
{
public:
	MDX_GeometryPass() : Technique("shaders/MDX") {}

	//

	inline void SetColorTextureUnit(int TextureUnit)
	{
		setTexture("gColorMap", TextureUnit);
	}

	inline void SetSpecularTextureUnit(int TextureUnit)
	{
		setTexture("gSpecularMap", TextureUnit);
	}
};