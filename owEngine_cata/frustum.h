#pragma once

struct Plane
{
	float a, b, c, d;
	void normalize();
};

struct Frustum
{
	Plane planes[6];

	void retrieve();

	bool contains(const vec3 &v) const;
	bool intersects(const vec3 &v1, const vec3 &v2) const;
	bool intersectsSphere(cvec3 v, const float rad) const;
};

