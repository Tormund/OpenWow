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