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
	uint8_t type;
	uint8_t useAtten;
	uint8_t pad[2];
	uint32_t color;
	vec3 pos;
	float intensity;
	float attenStart;
	float attenEnd;
	float unk[4];

	//
	static const uint32_t __size = 48;
};

class WMOLight
{
public:
	WMOLight(File& f);

	void setup(GLint light);

	static void setupOnce(GLint light, vec3 dir, vec4 lcol);

public:
	vec4 fcolor;

public:
	WMOLightDef lightDef;
};