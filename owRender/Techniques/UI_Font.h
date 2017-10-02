#pragma once

#include "technique.h"

class UI_Font : public Technique
{
public:
	UI_Font();
	virtual bool Init();

	//

	inline void SetFontTexture(int texture)
	{
		setInt("gFontTexture", texture);
	}

	inline void SetFontColor(cvec3 _color)
	{
		setVec3("gFontColor", _color);
	}

	inline void SetCharOffset(cvec2 _color)
	{
		setVec2("gOffset", _color);
	}
};