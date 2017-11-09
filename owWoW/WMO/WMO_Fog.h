#pragma once

struct WMOFogDef
{
	struct
	{
		uint32 flag_infinite_radius : 1; // F_IEBLEND: Ignore radius in CWorldView::QueryCameraFog
		uint32 : 3;                      // unused as of 7.0.1.20994
		uint32 flag_0x10 : 1;
		uint32 : 27;                     // unused as of 7.0.1.20994
	} flags;

	vec3 position;
	float smallerRadius; // Smaller radius
	float largerRadius;  // Larger radius

	// Fog
	struct
	{
		float end;			// Fog end
		float startScalar;	// Fog start multiplier (0..1)
		CBgra color;	// The back buffer is also cleared to this colour 
	} fog;

	// Underwater fog
	struct
	{
		float end;
		float startScalar;
		CBgra color;
	} underwater_fog;

	//
	static const uint32 __size = 48;
};


struct WMOFog
{
public:
	WMOFog(File& f);

	void setup();

public:
	vec4 color;

public:
	WMOFogDef fogDef;
};