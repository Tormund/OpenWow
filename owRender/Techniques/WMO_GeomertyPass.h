#pragma once

#include "technique.h"

class WMO_GeomertyPass : public Technique
{
public:
	WMO_GeomertyPass();
	virtual bool Init();

	inline void SetColorTextureUnit(int TextureUnit)
	{
		setInt("gColorMap", TextureUnit);
	}

	inline void SetSpecularTextureUnit(int TextureUnit)
	{
		setInt("gSpecularMap", TextureUnit);
	}

	//

	void SetHasMOCV(bool hasMOCV)
	{
		setBool("gHasMOCV", hasMOCV);
	}

	void SetDiffuseColor(vec3 _Color)
	{
		setVec3("gDiffuseColor", _Color);
	}
};