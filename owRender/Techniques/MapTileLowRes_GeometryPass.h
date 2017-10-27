#pragma once

#include "technique.h"

class MapTileLowRes_GeometryPass : public Technique
{
public:
	MapTileLowRes_GeometryPass() : Technique("shaders/MapTileLowRes") {}

	void SetShadowColor(vec3 _Color)
	{
		setVec3("gColor", _Color);
	}
};