#pragma once

class BoundingBox
{
public:
	BoundingBox();

	void Calculate(const vec3* _verts, uint32_t _count, bool _needConvert = true);
	void SetBounds(cvec3 _min, cvec3 _max, bool _needConvert = true);

	inline vec3 GetCenter() { return center; }
	inline float GetRadius() { return radius; }

	inline const vec3 Min() { return m_BoundsMin; }
	inline const vec3 Max() { return m_BoundsMax; }

private:
	void CalculateInternal();

private:
	vec3 center;
	float radius;
	vec3 m_BoundsMin, m_BoundsMax;
};