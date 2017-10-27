#pragma once

enum LightType
{
	OMNI_LGT,
	SPOT_LGT,
	DIRECT_LGT,
	AMBIENT_LGT
};

struct WMOLightDef
{
	uint8 type;
	uint8 useAtten;
	uint8 pad[2];
	uint32 color;
	vec3 pos;
	float intensity;
	float attenStart;
	float attenEnd;
	float unk[4];

	//
	static const uint32 __size = 48;
};

class WMOLight
{
public:
	WMOLight(File& f);

	void setup(uint32 light);

	static void setupOnce(uint32 light, vec3 dir, vec4 lcol);

public:
	vec4 fcolor;

public:
	WMOLightDef lightDef;
};