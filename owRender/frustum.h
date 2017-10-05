#pragma once

struct Plane2
{
	inline void normalize()
	{
		float len = sqrtf(a * a + b * b + c * c);
		a /= len;
		b /= len;
		c /= len;
		d /= len;
	}

	inline bool Contains(cvec3 _point) const
	{
		return (a * _point.x + b * _point.y + c * _point.z + d) > 0;
	}

	inline float Distance(cvec3 _point) const
	{
		return a * _point.x + b * _point.y + c * _point.z + d;
	}

	float a, b, c, d;
};

struct Frustum2
{
	Plane2 planes[6];

	void retrieve();

	bool contains(cvec3 v) const;
	bool intersects(cvec3 v1, cvec3 v2) const;
	bool intersects2(cvec3 v1, cvec3 v2) const;
	bool intersectsSphere(cvec3 v, const float rad) const;
};

