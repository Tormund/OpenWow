#pragma once

#include "vector3.h"

#include <cmath>

// ------------------------------------------------------------------------------------------------
/** Transformation of a vector by a 3x3 matrix */
template <typename T>
inline Vector3t<T> operator * (const Matrix3x3t<T>& pMatrix, const Vector3t<T>& pVector) {
	Vector3t<T> res;
	res.x = pMatrix.a1 * pVector.x + pMatrix.a2 * pVector.y + pMatrix.a3 * pVector.z;
	res.y = pMatrix.b1 * pVector.x + pMatrix.b2 * pVector.y + pMatrix.b3 * pVector.z;
	res.z = pMatrix.c1 * pVector.x + pMatrix.c2 * pVector.y + pMatrix.c3 * pVector.z;
	return res;
}

// ------------------------------------------------------------------------------------------------
/** Transformation of a vector by a 4x4 matrix */
template <typename T>
inline Vector3t<T> operator * (const Matrix4x4t<T>& pMatrix, const Vector3t<T>& pVector) {
	Vector3t<T> res;
	res.x = pMatrix.a1 * pVector.x + pMatrix.a2 * pVector.y + pMatrix.a3 * pVector.z + pMatrix.a4;
	res.y = pMatrix.b1 * pVector.x + pMatrix.b2 * pVector.y + pMatrix.b3 * pVector.z + pMatrix.b4;
	res.z = pMatrix.c1 * pVector.x + pMatrix.c2 * pVector.y + pMatrix.c3 * pVector.z + pMatrix.c4;
	return res;
}
// ------------------------------------------------------------------------------------------------
template <typename T>
template <typename TOther>
Vector3t<T>::operator Vector3t<TOther>() const {
	return Vector3t<TOther>(static_cast<TOther>(x), static_cast<TOther>(y), static_cast<TOther>(z));
}
// ------------------------------------------------------------------------------------------------
template <typename T>
FORCE_INLINE void Vector3t<T>::Set(T pX, T pY, T pZ) {
	x = pX; y = pY; z = pZ;
}
// ------------------------------------------------------------------------------------------------
template <typename T>
FORCE_INLINE T Vector3t<T>::SquareLength() const {
	return x*x + y*y + z*z;
}
// ------------------------------------------------------------------------------------------------
template <typename T>
FORCE_INLINE T Vector3t<T>::Length() const {
	return std::sqrt(SquareLength());
}
// ------------------------------------------------------------------------------------------------
template <typename T>
FORCE_INLINE Vector3t<T>& Vector3t<T>::Normalize() {
	*this /= Length(); return *this;
}
// ------------------------------------------------------------------------------------------------
template <typename T>
FORCE_INLINE Vector3t<T>& Vector3t<T>::NormalizeSafe() {
	T len = Length();
	if(len > static_cast<T>(0))
		*this /= len;
	return *this;
}
// ------------------------------------------------------------------------------------------------
template <typename T>
FORCE_INLINE const Vector3t<T>& Vector3t<T>::operator += (const Vector3t<T>& o) {
	x += o.x; y += o.y; z += o.z; return *this;
}
// ------------------------------------------------------------------------------------------------
template <typename T>
FORCE_INLINE const Vector3t<T>& Vector3t<T>::operator -= (const Vector3t<T>& o) {
	x -= o.x; y -= o.y; z -= o.z; return *this;
}
// ------------------------------------------------------------------------------------------------
template <typename T>
FORCE_INLINE const Vector3t<T>& Vector3t<T>::operator *= (T f) {
	x *= f; y *= f; z *= f; return *this;
}
// ------------------------------------------------------------------------------------------------
template <typename T>
FORCE_INLINE const Vector3t<T>& Vector3t<T>::operator /= (T f) {
	x /= f; y /= f; z /= f; return *this;
}
// ------------------------------------------------------------------------------------------------
template <typename T>
FORCE_INLINE Vector3t<T>& Vector3t<T>::operator *= (const Matrix3x3t<T>& mat) {
	return(*this = mat * (*this));
}
// ------------------------------------------------------------------------------------------------
template <typename T>
FORCE_INLINE Vector3t<T>& Vector3t<T>::operator *= (const Matrix4x4t<T>& mat) {
	return(*this = mat * (*this));
}
// ------------------------------------------------------------------------------------------------
template <typename T>
FORCE_INLINE T Vector3t<T>::operator[](unsigned int i) const {
	return *(&x + i);
}
// ------------------------------------------------------------------------------------------------
template <typename T>
FORCE_INLINE T& Vector3t<T>::operator[](unsigned int i) {
	return *(&x + i);
}
// ------------------------------------------------------------------------------------------------
template <typename T>
FORCE_INLINE bool Vector3t<T>::operator== (const Vector3t<T>& other) const {
	return x == other.x && y == other.y && z == other.z;
}
// ------------------------------------------------------------------------------------------------
template <typename T>
FORCE_INLINE bool Vector3t<T>::operator!= (const Vector3t<T>& other) const {
	return x != other.x || y != other.y || z != other.z;
}
// ---------------------------------------------------------------------------
template<typename T>
FORCE_INLINE bool Vector3t<T>::Equal(const Vector3t<T>& other, T epsilon) const {
	return
		std::abs(x - other.x) <= epsilon &&
		std::abs(y - other.y) <= epsilon &&
		std::abs(z - other.z) <= epsilon;
}
// ------------------------------------------------------------------------------------------------
template <typename T>
FORCE_INLINE bool Vector3t<T>::operator < (const Vector3t<T>& other) const {
	return x != other.x ? x < other.x : y != other.y ? y < other.y : z < other.z;
}
// ------------------------------------------------------------------------------------------------
template <typename T>
FORCE_INLINE const Vector3t<T> Vector3t<T>::SymMul(const Vector3t<T>& o) {
	return Vector3t<T>(x*o.x, y*o.y, z*o.z);
}
// ------------------------------------------------------------------------------------------------
// symmetric addition
template <typename T>
FORCE_INLINE Vector3t<T> operator + (const Vector3t<T>& v1, const Vector3t<T>& v2) {
	return Vector3t<T>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}
// ------------------------------------------------------------------------------------------------
// symmetric subtraction
template <typename T>
FORCE_INLINE Vector3t<T> operator - (const Vector3t<T>& v1, const Vector3t<T>& v2) {
	return Vector3t<T>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}
// ------------------------------------------------------------------------------------------------
// scalar product
template <typename T>
FORCE_INLINE T operator * (const Vector3t<T>& v1, const Vector3t<T>& v2) {
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}
// ------------------------------------------------------------------------------------------------
// scalar multiplication
template <typename T>
FORCE_INLINE Vector3t<T> operator * (T f, const Vector3t<T>& v) {
	return Vector3t<T>(f*v.x, f*v.y, f*v.z);
}
// ------------------------------------------------------------------------------------------------
// and the other way around
template <typename T>
FORCE_INLINE  Vector3t<T> operator * (const Vector3t<T>& v, T f) {
	return Vector3t<T>(f*v.x, f*v.y, f*v.z);
}
// ------------------------------------------------------------------------------------------------
// scalar division
template <typename T>
FORCE_INLINE  Vector3t<T> operator / (const Vector3t<T>& v, T f) {
	return v * (1 / f);
}
// ------------------------------------------------------------------------------------------------
// vector division
template <typename T>
FORCE_INLINE  Vector3t<T> operator / (const Vector3t<T>& v, const Vector3t<T>& v2) {
	return Vector3t<T>(v.x / v2.x, v.y / v2.y, v.z / v2.z);
}
// ------------------------------------------------------------------------------------------------
// cross product
template <typename T>
FORCE_INLINE  Vector3t<T> operator ^ (const Vector3t<T>& v1, const Vector3t<T>& v2) {
	return Vector3t<T>(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}
// ------------------------------------------------------------------------------------------------
// vector negation
template <typename T>
FORCE_INLINE  Vector3t<T> operator - (const Vector3t<T>& v) {
	return Vector3t<T>(-v.x, -v.y, -v.z);
}
