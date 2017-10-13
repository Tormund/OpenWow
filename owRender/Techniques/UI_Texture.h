#pragma once

#include "technique.h"

class UI_Texture : public Technique
{
public:
	UI_Texture();
	virtual bool Init();

	//

	inline void SetTexture(int texture)
	{
		setTexture("gTexture", texture);
	}
};