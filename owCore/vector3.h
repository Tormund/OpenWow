#pragma once

#include <cmath>

#include "pushpack.h"

template<typename T> 
class Matrix3x3t;

template<typename T> 
class Matrix4x4t;

template <typename T>
class Vector3t {
public:
	Vector3t() : x(), y(), z() { }
	Vector3t(T _x, T _y, T _z) : x(_x), y(_y), z(_z) { }
	explicit Vector3t(T _xyz) : x(_xyz), y(_xyz), z(_xyz) { }
	Vector3t(const Vector3t& o) : x(o.x), y(o.y), z(o.z) { }

public:
	const Vector3t& operator += (const Vector3t& o);
	const Vector3t& operator -= (const Vector3t& o);
	const Vector3t& operator *= (T f);
	const Vector3t& operator /= (T f);

	// transform vector by matrix
	Vector3t& operator *= (const Matrix3x3t<T>& mat);
	Vector3t& operator *= (const Matrix4x4t<T>& mat);

	// access a single element
	T operator[](unsigned int i) const;
	T& operator[](unsigned int i);

	// comparison
	bool operator== (const Vector3t& other) const;
	bool operator!= (const Vector3t& other) const;
	bool operator < (const Vector3t& other) const;

	bool Equal(const Vector3t& other, T epsilon = 1e-6) const;

	template <typename T2>
	operator Vector3t<T2>() const;

public:

	/** @brief Set the components of a vector
	 *  @param pX X component
	 *  @param pY Y component
	 *  @param pZ Z component  */
	void Set(T pX, T pY, T pZ);

	/** @brief Get the squared length of the vector
	 *  @return Square length */
	T SquareLength() const;


	/** @brief Get the length of the vector
	 *  @return length */
	T Length() const;


	/** @brief Normalize the vector */
	Vector3t& Normalize();

	/** @brief Normalize the vector with extra check for zero vectors */
	Vector3t& NormalizeSafe();

	/** @brief Componentwise multiplication of two vectors
	 *
	 *  Note that vec*vec yields the dot product.
	 *  @param o Second factor */
	const Vector3t SymMul(const Vector3t& o);

	T x, y, z;
};

typedef Vector3t<float> Vector3D;

#include "poppack.h"

