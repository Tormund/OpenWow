#pragma once

#include "technique.h"

class Debug_GeometryPass : public Technique
{
public:
	Debug_GeometryPass();
	virtual bool Init();

	inline void SetColor4(cvec4 _color)
	{
		setVec4("gColor", _color);
	}
};