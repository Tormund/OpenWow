#pragma once

#include "MDX_Headers.h"

class MDX_Part_TextureAnim
{
public:
	void init(File& f, M2TextureTransform& mta, uint32* global);
	void calc(int anim, int time);

	Matrix4f getValue()
	{
		return matrix;
	}

private:
	vec3 tval, sval;
	Quaternion rval;

	Animated<vec3> trans, scale;
	Animated<Quaternion> roll;

	Matrix4f matrix;
	//

	friend class MDX;
	friend class MDX_Skin_Batch;
};