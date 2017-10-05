#pragma once

class BoundingBox2
{
public:
	BoundingBox2();

	void Calculate(const vec3* _verts, uint32 _count, bool _needConvert = true);
	void SetBounds(cvec3 _min, cvec3 _max, bool _needConvert = true);

	inline vec3 GetCenter() { return m_Center; }
	inline float GetRadius() { return m_Radius; }

	inline const vec3 Min() { return m_BoundsMin; }
	inline const vec3 Max() { return m_BoundsMax; }

private:
	void CalculateInternal();

private:
	vec3 m_Center;
	float m_Radius;
	vec3 m_BoundsMin;
	vec3 m_BoundsMax;
};