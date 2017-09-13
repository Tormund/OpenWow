#pragma once

struct Plane
{
	float a, b, c, d;
	void normalize();
};

struct Frustum
{
	Plane planes[6];

	OW_RENDER_DLL_API void retrieve();

	OW_RENDER_DLL_API bool contains(cvec3 v) const;
	OW_RENDER_DLL_API bool intersects(cvec3 v1, cvec3 v2) const;
	OW_RENDER_DLL_API bool intersectsSphere(cvec3 v, const float rad) const;
};

