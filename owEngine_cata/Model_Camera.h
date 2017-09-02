#pragma once

#include "modelheaders.h"
#include "animated.h"

struct ModelCamera
{
public:
	void init(File& f, ModelCameraDef& mcd, uint32_t * global);
	void setup(int time = 0);

	ModelCamera() :ok(false) {}

public:
	bool ok;

	vec3 pos, target;
	float nearclip, farclip, fov;
	Animated<vec3> tPos, tTarget;
	Animated<float> rot;
};