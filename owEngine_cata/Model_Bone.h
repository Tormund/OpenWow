#pragma once

#include "modelheaders.h"
#include "quaternion.h"
#include "matrix.h"
#include "animated.h"

class Bone
{
public:
	void init(File &f, ModelBoneDef &b, uint32_t *global, File *animfiles);
	void calcMatrix(Bone* allbones, int anim, int time);

private:
	Animated<vec3> trans;
	//Animated<Quaternion> rot;
	Animated<Quaternion, PACK_QUATERNION, Quat16ToQuat32> rot;
	Animated<vec3> scale;

public:
	vec3 pivot, transPivot;
	int16_t parent;

	bool billboard;
	Matrix mat;
	Matrix mrot;

	bool calc;
};