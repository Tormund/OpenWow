#pragma once

#include "quaternion.h"

struct Particle
{
	vec3 pos, speed, down, origin, dir;
	vec3 corners[4];
	//vec3 tpos;
	float size, life, maxlife;
	uint32_t m_TileExists;
	Vec4D color;
};
