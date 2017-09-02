#include "stdafx.h"

// Include
#include "quaternion.h"

// General
#include "ModelsUtils.h"

// Additional
#include "matrix.h"

vec3 fixCoordSystem(vec3 v)
{
	return vec3(v.x, v.z, -v.y);
}

vec3 fixCoordSystem2(vec3 v)
{
	return vec3(v.x, v.z, v.y);
}

Quaternion fixCoordSystemQuat(Quaternion v)
{
	return Quaternion(-v.x, -v.z, v.y, v.w);
}

void glQuaternionRotate(cvec3 vdir, float w)
{
	Matrix m;
	Quaternion q(vdir, w);
	m.quaternionRotate(q);
	glMultMatrixf(m);
}