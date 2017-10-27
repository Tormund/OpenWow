#pragma once

#include "technique.h"

class WMO_GeomertyPass : public Technique
{
public:
	WMO_GeomertyPass() : Technique("shaders/WMO") {}

	inline void SetColorTextureUnit(int TextureUnit)
	{
		setTexture("gColorMap", TextureUnit);
	}

	inline void SetSpecularTextureUnit(int TextureUnit)
	{
		setTexture("gSpecularMap", TextureUnit);
	}

	//

	void SetHasMOCV(bool hasMOCV)
	{
		setInt("gHasMOCV", hasMOCV);
	}

	void SetDiffuseColor(vec3 _Color)
	{
		setVec3("gDiffuseColor", _Color);
	}
};