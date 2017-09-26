#pragma once

#include "technique.h"

class Billboard_GeometryPass : public Technique
{
public:
	Billboard_GeometryPass();
	virtual bool Init();

	//

	inline void SetColorTextureUnit(int TextureUnit)
	{
		setInt("gColorMap", TextureUnit);
	}

	inline void SetSpecularTextureUnit(int TextureUnit)
	{
		setInt("gSpecularMap", TextureUnit);
	}
};