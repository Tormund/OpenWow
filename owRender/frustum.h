#pragma once

struct Plane
{
	inline void normalize()
	{
		float len = sqrtf(a*a + b*b + c*c);
		a /= len;
		b /= len;
		c /= len;
		d /= len;
	}

	float a, b, c, d;
};

struct Frustum
{
	Plane planes[6];

	void retrieve();

	bool contains(cvec3 v) const;
	bool intersects(cvec3 v1, cvec3 v2) const;
	bool intersectsSphere(cvec3 v, const float rad) const;
};

