#pragma once

inline void glQuaternionRotate(cvec3 vdir, float w)
{
	Matrix m;
	Quaternion q(w, vdir);
	m.quaternionRotate(q);
	glMultMatrixf(m);
}