#pragma once

#include "MDX_Headers.h"

class MDX_Part_Camera
{
public:
	void init(File& f, M2Camera& mcd, uint32* global);
	void setup(int time = 0);

	Camera* GetCamera() { return &camera; }

private:
	float farclip;
	float nearclip;

	Animated<vec3> tPos;
	vec3 pos;

	Animated<vec3> tTarget;
	vec3 target;

	Animated<float> tRoll;
	float roll;

	Animated<float> tFov;
	float fov;

	Camera camera;

	//

	friend class MDX;
};