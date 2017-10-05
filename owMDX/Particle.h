#pragma once

struct Particle
{
	vec3 pos, speed, down, origin, dir;
	vec3 corners[4];
	//vec3 tpos;
	float size, life, maxlife;
	uint32 m_TileExists;
	vec4 color;
};
