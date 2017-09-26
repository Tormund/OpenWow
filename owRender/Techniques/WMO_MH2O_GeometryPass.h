#pragma once

#include "technique.h"

class WMO_MH2O_GeometryPass : public Technique
{
public:
	WMO_MH2O_GeometryPass();
	virtual bool Init();

	inline void SetColorTextureUnit(int TextureUnit)
	{
		setInt("gColorMap", TextureUnit);
	}

	inline void SetSpecularTextureUnit(int TextureUnit)
	{
		setInt("gSpecularMap", TextureUnit);
	}

	void SetWaterColor(vec3 _Color)
	{
		setVec3("gColor", _Color);
	}
};