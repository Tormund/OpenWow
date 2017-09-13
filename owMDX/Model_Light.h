#pragma once

struct ModelLightDef
{
	int16_t type;
	int16_t bone;
	vec3 pos;
	AnimationBlock ambColor;
	AnimationBlock ambIntensity;
	AnimationBlock color;
	AnimationBlock intensity;
	AnimationBlock attStart;
	AnimationBlock attEnd;
	AnimationBlock unk1;
};

enum ModelLightTypes
{
	MODELLIGHT_DIRECTIONAL = 0,
	MODELLIGHT_POINT = 1
};

class ModelLight
{
public:
	OW_MDX_DLL_API void init(File& f, ModelLightDef& mld, uint32_t * global);
	OW_MDX_DLL_API void setup(int time, GLuint l);

public:
	int type, parent;
	vec3 pos, tpos, dir, tdir;
	Animated<vec3> diffColor, ambColor;
	Animated<float> diffIntensity, ambIntensity;
};