#pragma once

class BoundingBox
{
public:
	void Init(vec3* _verts, uint32_t _count, bool _needConvert = true);
	void SetBounds(cvec3 _vec1, cvec3 _vec2, bool _needConvert = true);

	inline vec3 GetCenter() { return center; }
	inline float GetRadius() { return radius; }

private:
	vec3 b1, b2;
	vec3 center;
	float radius;
	vec3 vmin, vmax;
};