#pragma once

inline vec3 fixCoordSystem(cvec3 v)
{
	return vec3(v.x, v.z, -v.y);
}

inline vec3 fixCoordSystem2(cvec3 v)
{
	return vec3(v.x, v.z, v.y);
}

inline Quaternion fixCoordSystemQuat(cQuaternion v)
{
	return Quaternion(v.w, -v.x, -v.z, v.y);
}