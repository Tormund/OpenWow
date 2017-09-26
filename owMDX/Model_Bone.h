#pragma once

#define	MODELBONE_BILLBOARD	8
#define	MODELBONE_TRANSFORM	512

#include "ModelHeaders.h"

class Bone
{
public:
	void init(File& f, M2CompBone& b, uint32_t* global, File* animfiles);
	void calcMatrix(Bone* allbones, int anim, int time);

private:
	Animated<vec3> trans;
	Animated<Quaternion, M2CompQuat, Quat16ToQuat32> roll;
	Animated<vec3> scale;

public:
	vec3 pivot, transPivot;
	int16_t parent;

	bool billboard;
	Matrix mat;
	Matrix mrot;

	bool calc;
};