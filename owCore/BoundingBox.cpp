#include "stdafx.h"

// General
#include "BoundingBox.h"

void BoundingBox::Init(vec3* _verts, uint32_t _count, bool _needConvert)
{
	vmin = vec3(9999999.0f, 9999999.0f, 9999999.0f);
	vmax = vec3(-9999999.0f, -9999999.0f, -9999999.0f);

	for (uint32_t i = 0; i < _count; i++)
	{
		vec3 v = _needConvert ? From_XYZ_To_XZminusY_RET(_verts[i]) : _verts[i];

		if (v.x < vmin.x) vmin.x = v.x;
		if (v.y < vmin.y) vmin.y = v.y;
		if (v.z < vmin.z) vmin.z = v.z;
		if (v.x > vmax.x) vmax.x = v.x;
		if (v.y > vmax.y) vmax.y = v.y;
		if (v.z > vmax.z) vmax.z = v.z;
	}

	center = (vmax + vmin) * 0.5f;
	radius = glm::length(vmax - center);
}

void BoundingBox::SetBounds(cvec3 _vec1, cvec3 _vec2, bool _needConvert)
{
	b1 = _needConvert ? From_XYZ_To_XZminusY_RET(_vec1) : _vec1;
	b2 = _needConvert ? From_XYZ_To_XZminusY_RET(_vec2) : _vec2;
}
