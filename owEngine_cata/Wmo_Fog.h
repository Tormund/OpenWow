#pragma once

#include "WowTypes.h"

struct WMOFog
{
	uint32_t flags;
	vec3 position;
	float smallerRadius; // Smaller radius
	float largerRadius; // Larger radius

	// Fog
	struct
	{
		float end;			// Fog end
		float startScalar;	// Fog start multiplier (0..1)
		CImVector color;	// The back buffer is also cleared to this colour 
	} fog;

	// Underwater fog
	struct
	{
		float end;
		float startScalar;
		CImVector color;
	} underwater_fog;

	static const size_t __size = 0x30;

public:
	void init(File& f);
	void setup();

public:
	vec4 color;
};