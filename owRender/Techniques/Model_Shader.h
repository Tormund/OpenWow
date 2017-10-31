#pragma once

#include "technique.h"

class Model_Shader : public Technique
{
public:
	Model_Shader() : Technique("shaders/Models/Model") {}

	// Colors

	inline void SetColor(vec4 _color)
	{
		setVec4("gColor", _color);
	}

	// Textures
 
	inline void SetDiffuseTexture(int TextureUnit)
	{
		setTexture("gDiffuseTexture", TextureUnit);
	}

	inline void SetSpecularTexture(int TextureUnit)
	{
		setTexture("gSpecularTexture", TextureUnit);
	}

	// Blending & Alpha

	inline void SetAlpha(float _alpha)
	{
		setFloat("gAlpha", _alpha);
	}

	inline void SetBlendMode(int _blendMode)
	{
		setInt("gBlendMode", _blendMode);
	}

	// Texture Weight
	inline void SetTextureWeight(float _weight)
	{
		setFloat("gTextureWeight", _weight);
	}


	// Billboard
	inline void SetBillboard(int _flag)
	{
		setInt("gBillboard", _flag);
	}

	// TextureAnim
	inline void SetTextureAnimEnable(int _flag)
	{
		setInt("gTextureAnimEnable", _flag);
	}

	inline void SetTextureAnimMatrix(mat4 _matrix)
	{
		setMat4("gTextureAnimMatrix", _matrix);
	}
};
