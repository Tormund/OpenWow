#pragma once

#include "technique.h"

class Water_Pass : public Technique
{
public:
	Water_Pass() : Technique("shaders/Water") {}

	inline void SetColorTextureUnit(int TextureUnit)
	{
		setTexture("gColorMap", TextureUnit);
	}

	inline void SetSpecularTextureUnit(int TextureUnit)
	{
		setTexture("gSpecularMap", TextureUnit);
	}

	void SetWaterColorLight(cvec3 _Color)
	{
		setVec3("gColorLight", _Color);
	}

	void SetWaterColorDark(cvec3 _Color)
	{
		setVec3("gColorDark", _Color);
	}

    void SetShallowAlpha(float _value)
    {
        setFloat("gShallowAlpha", _value);
    }

    void SetDeepAlpha(float _value)
    {
        setFloat("gDeepAlpha", _value);
    }
};