#pragma once

template <typename T>
inline T Min(const T& _value, const T _min)
{
	return (_value < _min) ? _value : _min;
}

template <typename T>
inline T Max(const T& _value, const T _max)
{
	return (_value > _max) ? _value : _max;
}

template <typename T>
inline T Clamp(const T& _value, const T _min, const T _max)
{
	return Max(_min, Min(_value, _max));
}

inline vec3 fixCoordSystem2(cvec3 v)
{
	return vec3(v.x, v.z, v.y);
}

inline quat fixCoordSystemQuat(C4ImQuaternion v)
{
	return quat(v.w, -v.x, -v.z, v.y);
}

inline quat fixCoordSystemQuat(C4Quaternion v)
{
	return quat(v.w, -v.x, -v.z, v.y);
}

inline quat fixCoordSystemQuat(cquat v)
{
	return quat(v.w, -v.x, -v.z, v.y);
}

inline void rotate(float x0, float y0, float *x, float *y, float angle)
{
	float xa = *x - x0;
	float ya = *y - y0;
	*x = xa * cosf(angle) - ya * sinf(angle) + x0;
	*y = xa * sinf(angle) + ya * cosf(angle) + y0;
}

