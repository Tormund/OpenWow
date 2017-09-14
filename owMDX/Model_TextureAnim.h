#pragma once

#include "ModelHeaders.h"

class ModelTextureAnim
{
public:
	OW_MDX_DLL_API void init(File& f, M2TextureTransform& mta, uint32_t* global);
	OW_MDX_DLL_API void calc(int anim, int time);
	OW_MDX_DLL_API void setup(int anim);

public:
	vec3 tval, rval, sval;

private:
	Animated<vec3> trans, roll, scale;
};