#pragma once

#include "technique.h"

class Debug_GeometryPass : public Technique
{
public:
	Debug_GeometryPass();
	virtual bool Init();

	inline void SetColor(cvec3 _color)
	{
		setVec3("gColor", _color);
	}
};