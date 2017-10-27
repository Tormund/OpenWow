#pragma once

#include "technique.h"

class UI_Texture : public Technique
{
public:
	UI_Texture() : Technique("shaders/UI_Texture") {}

	//

	inline void SetTexture(int texture)
	{
		setTexture("gTexture", texture);
	}
};