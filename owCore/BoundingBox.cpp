#include "stdafx.h"

// General
#include "BoundingBox.h"

#define LOW_FLOAT  (-9999999.0f)
#define HIGH_FLOAT ( 9999999.0f)

BoundingBox2::BoundingBox2()
{
	m_Center = vec3(LOW_FLOAT, LOW_FLOAT, LOW_FLOAT);
	m_Radius = LOW_FLOAT;
	m_BoundsMin = vec3(HIGH_FLOAT, HIGH_FLOAT, HIGH_FLOAT);
	m_BoundsMax = vec3(LOW_FLOAT, LOW_FLOAT, LOW_FLOAT);
}

void BoundingBox2::Calculate(const vec3* _verts, uint32 _count, bool _needConvert)
{
	for (uint32 i = 0; i < _count; i++)
	{
		vec3 v = _needConvert ? From_XYZ_To_XZminusY_RET(_verts[i]) : _verts[i];

		if (v.x < m_BoundsMin.x) m_BoundsMin.x = v.x;
		if (v.y < m_BoundsMin.y) m_BoundsMin.y = v.y;
		if (v.z < m_BoundsMin.z) m_BoundsMin.z = v.z;
		if (v.x > m_BoundsMax.x) m_BoundsMax.x = v.x;
		if (v.y > m_BoundsMax.y) m_BoundsMax.y = v.y;
		if (v.z > m_BoundsMax.z) m_BoundsMax.z = v.z;
	}

	CalculateInternal();
}

void BoundingBox2::SetBounds(cvec3 _min, cvec3 _max, bool _needConvert)
{
	m_BoundsMin = _min;
	m_BoundsMax = _max;

	// Fix bounding box
	if (_needConvert)
	{
		From_XYZ_To_XZminusY(m_BoundsMin);
		From_XYZ_To_XZminusY(m_BoundsMax);
		std::swap(m_BoundsMin.z, m_BoundsMax.z);
	}

	assert1(m_BoundsMin.x < m_BoundsMax.x && m_BoundsMin.y < m_BoundsMax.y && m_BoundsMin.z < m_BoundsMax.z);

	CalculateInternal();
}

void BoundingBox2::CalculateInternal()
{
	m_Center = (m_BoundsMin + m_BoundsMax) * 0.5f;
	m_Radius = glm::length(m_BoundsMax - m_Center);

	assert1(m_Radius > 0);
}
