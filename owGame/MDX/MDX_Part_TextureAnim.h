#pragma once

#include "MDX_Headers.h"

class MDX_Part_TextureAnim
{
public:
	void init(File& f, M2TextureTransform& mta, uint32* global);
	void calc(int anim, int time);
	void setup(int anim);

private:
	vec3 tval, rval, sval;

	Animated<vec3> trans, roll, scale;

	//

	friend class MDX;
	friend class ModelRenderPass;
};