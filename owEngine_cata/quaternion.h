#ifndef QUATERNION_H
#define QUATERNION_H

class Vec4D
{
public:
	float x, y, z, w;

	Vec4D(float x0 = 0.0f, float y0 = 0.0f, float z0 = 0.0f, float w0 = 0.0f) : x(x0), y(y0), z(z0), w(w0) {}

	Vec4D(const Vec4D& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

	Vec4D(cvec3 v, const float w0) : x(v.x), y(v.y), z(v.z), w(w0) {}

	Vec4D& operator= (const Vec4D &v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	}

	const Vec4D operator+ (const Vec4D &v) const
	{
		Vec4D r(x + v.x, y + v.y, z + v.z, w + v.w);
		return r;
	}

	const Vec4D operator- (const Vec4D &v) const
	{
		Vec4D r(x - v.x, y - v.y, z - v.z, w - v.w);
		return r;
	}

	const Vec4D operator* (float d) const
	{
		Vec4D r(x*d, y*d, z*d, w*d);
		return r;
	}

	friend Vec4D operator* (float d, const Vec4D& v)
	{
		return v * d;
	}

	Vec4D& operator+= (const Vec4D &v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}

	Vec4D& operator-= (const Vec4D &v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}

	Vec4D& operator*= (float d)
	{
		x *= d;
		y *= d;
		z *= d;
		w *= d;
		return *this;
	}

	float operator* (const Vec4D &v) const
	{
		return x*v.x + y*v.y + z*v.z + w*v.w;
	}

	float lengthSquared() const
	{
		return x*x + y*y + z*z + w*w;
	}

	float length() const
	{
		return sqrt(x*x + y*y + z*z + w*w);
	}

	Vec4D& normalize()
	{
		this->operator*= (1.0f / length());
		return *this;
	}

	operator float*()
	{
		return (float*)this;
	}

	vec3 xyz() const
	{
		return vec3(x, y, z);
	}
};

inline Vec4D fromARGB(uint32_t color)
{
	const float a = ((color & 0xFF000000) >> 24) / 255.0f;
	const float r = ((color & 0x00FF0000) >> 16) / 255.0f;
	const float g = ((color & 0x0000FF00) >> 8) / 255.0f;
	const float b = ((color & 0x000000FF)) / 255.0f;
	return Vec4D(r, g, b, a);
}

inline Vec4D fromBGRA(uint32_t color)
{
	const float b = ((color & 0xFF000000) >> 24) / 255.0f;
	const float g = ((color & 0x00FF0000) >> 16) / 255.0f;
	const float r = ((color & 0x0000FF00) >> 8) / 255.0f;
	const float a = ((color & 0x000000FF)) / 255.0f;
	return Vec4D(r, g, b, a);
}

class Quaternion : public Vec4D
{
public:
	Quaternion(float x0 = 0.0f, float y0 = 0.0f, float z0 = 0.0f, float w0 = 1.0f) : Vec4D(x0, y0, z0, w0) {}

	Quaternion(const Vec4D& v) : Vec4D(v) {}

	Quaternion(cvec3 v, const float w0) : Vec4D(v, w0) {}

	static const Quaternion slerp(const float r, const Quaternion &v1, const Quaternion &v2)
	{
		// SLERP
		float dot = v1*v2;

		if (fabs(dot) > 0.9995f)
		{
			// fall back to LERP
			return Quaternion::lerp(r, v1, v2);
		}

		float a = acosf(dot) * r;
		Quaternion q = (v2 - v1 * dot);
		q.normalize();

		return v1 * cosf(a) + q * sinf(a);
	}

	static const Quaternion lerp(const float r, const Quaternion &v1, const Quaternion &v2)
	{
		return v1*(1.0f - r) + v2*r;
	}

};

#endif

