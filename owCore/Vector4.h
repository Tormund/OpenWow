#pragma once

// Additional
#include <cmath>
#include "Vector3.h"

template <typename T>
class vec4 {
public:
	vec4() : x(0), y(0), z(0), w(0) { }
	vec4(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) { }

	template <class T2>
	vec4(const vec4<T2>& _other) : x(static_cast<T>(_other.x)), y(static_cast<T>(_other.y)), z(static_cast<T>(_other.z)), w(static_cast<T>(_other.w)) { }

	template <class T2, class T3>
	vec4(const vec3<T2>& _vec, const T3 _w) : x(static_cast<T>(_vec.x)), y(static_cast<T>(_vec.y)), z(static_cast<T>(_vec.z)), w(static_cast<T>(_w)) { }

	//

	void Clear() {
		x = y = z = w = 0;
	}

	float Length() const {
		return sqrtf(x*x + y*y + z*z + w*w);
	}

	T LengthSquared() const {
		return x*x + y*y + z*z + w*w;
	}

	vec4& Normalize() {
		return (*this /= Length());
	}

	// Assign
	template <class T2>
	vec4& operator= (const vec4<T2>& _other) {
		x = static_cast<T>(_other.x);
		y = static_cast<T>(_other.y);
		z = static_cast<T>(_other.z);
		w = static_cast<T>(_other.w);
		return *this;
	}

	// Equal (vector)
	template <class T2>
	bool operator==(const vec4<T2>& _other) const { return x == static_cast<T>(_other.x) && y == static_cast<T>(_other.y) && z == static_cast<T>(_other.z) && w == static_cast<T>(_other.w); }
	template <class T2>
	bool operator!=(const vec4<T2>& _other) const { return !(x == static_cast<T>(_other.x) && y == static_cast<T>(_other.y) && z == static_cast<T>(_other.z) && w == static_cast<T>(_other.w)); }

	// Plus (vector)
	template <class T2>
	vec4 operator+(const vec4<T2>& _other) const { return vec4(x + static_cast<T>(_other.x), y + static_cast<T>(_other.y), z + static_cast<T>(_other.z), w + static_cast<T>(_other.w)); }
	template <class T2>
	vec4& operator+=(const vec4<T2>& _other) {
		x += static_cast<T>(_other.x);
		y += static_cast<T>(_other.y);
		z += static_cast<T>(_other.z);
		w += static_cast<T>(_other.w);
		return *this;
	}

	// Plus (vector)
	template <class T2>
	vec4 operator-(const vec4<T2>& _other) const { return vec4(x - static_cast<T>(_other.x), y - static_cast<T>(_other.y), z - static_cast<T>(_other.z), w - static_cast<T>(_other.w)); }
	template <class T2>
	vec4& operator-=(const vec4<T2>& _other) {
		x -= static_cast<T>(_other.x);
		y -= static_cast<T>(_other.y);
		z -= static_cast<T>(_other.z);
		w -= static_cast<T>(_other.w);
		return *this;
	}

	// Mult (vector)
	template <class T2>
	vec4 operator*(const vec4<T2>& _value) const { return vec4(x *  static_cast<T>(_value.x), y *  static_cast<T>(_value.y), z *  static_cast<T>(_value.z), w * static_cast<T>(_value.w)); }
	template <class T2>
	vec4& operator*=(const vec4<T2>& _value) {
		x *= static_cast<T>(_value.x);
		y *= static_cast<T>(_value.y);
		z *= static_cast<T>(_value.z);
		w *= static_cast<T>(_other.w);
		return *this;
	}

	// Divide (vector)
	template <class T2>
	vec4 operator/(const vec4<T2>& _value) const { return vec4(x / static_cast<T>(_value.x), y / static_cast<T>(_value.y), z / static_cast<T>(_value.z), w / static_cast<T>(_value.w)); }
	template <class T2>
	vec4& operator/=(const vec4<T2>& _value) {
		x /= static_cast<T>(_value.x);
		y /= static_cast<T>(_value.y);
		z /= static_cast<T>(_value.z);
		w /*+*/ = static_cast<T>(_other.w);
		return *this;
	}

	// Mult (vec / const)
	template <class T2>
	vec4 operator*(const T2& _value) const { return vec4(x *  static_cast<T>(_value), y *  static_cast<T>(_value), z *  static_cast<T>(_value), w *  static_cast<T>(_value)); }
	template <class T2>
	vec4& operator*=(const T2& _value) {
		x *= static_cast<T>(_value);
		y *= static_cast<T>(_value);
		z *= static_cast<T>(_value);
		w *= static_cast<T>(_value);
		return *this;
	}

	// Divide (vec / const)
	template <class T2>
	vec4 operator/(const T2& _value) const { return vec4(x / static_cast<T>(_value), y / static_cast<T>(_value), z / static_cast<T>(_value), w / static_cast<T>(_value)); }
	template <class T2>
	vec4& operator/=(const T2& _value) {
		x /= static_cast<T>(_value);
		y /= static_cast<T>(_value);
		z /= static_cast<T>(_value);
		w /= static_cast<T>(_value);
		return *this;
	}

	// Type convertion
	template <class T2>
	vec4<T2> ToType() const {
		if(typeid(T) == typeid(T2))
			return *this;
		return vec4<T2>(static_cast<T2>(x), static_cast<T2>(y), static_cast<T2>(z), static_cast<T2>(w));
	}

	//

	/*friend vec4f operator* (float d, cvec4f v) {
		return v * d;
	}*/

	operator float*() {
		return (float*)this;
	}

	operator T() const {
		return x + y + z + w;
	}

	vec3<T> xyz() const {
		return vec3<T>(x, y, z);
	}

	T x;
	T y;
	T z;
	T w;
};


const vec4<int> VECTOR4I_ZERO = vec4<int>(0, 0, 0, 0);
const vec4<double> VECTOR4D_ZERO = vec4<double>(0.0, 0.0, 0.0, 0.0);
const vec4<float> VECTOR4F_ZERO = vec4<float>(0.0f, 0.0f, 0.0f, 0.0f);

typedef typename const vec4<int>& cvec4i;
typedef typename vec4<int> vec4i;

typedef typename const vec4<double>& cvec4d;
typedef typename vec4<double> vec4d;

typedef typename const vec4<float>& cvec4f;
typedef typename vec4<float> vec4f;