#pragma once

#include "vector2.h"

#include <cmath>

// ------------------------------------------------------------------------------------------------
template <typename TReal>
template <typename TOther>
Vector2t<TReal>::operator Vector2t<TOther>() const {
	return Vector2t<TOther>(static_cast<TOther>(x), static_cast<TOther>(y));
}
// ------------------------------------------------------------------------------------------------
template <typename TReal>
void Vector2t<TReal>::Set(TReal pX, TReal pY) {
	x = pX; y = pY;
}

// ------------------------------------------------------------------------------------------------
template <typename TReal>
TReal Vector2t<TReal>::SquareLength() const {
	return x*x + y*y;
}

// ------------------------------------------------------------------------------------------------
template <typename TReal>
TReal Vector2t<TReal>::Length() const {
	return std::sqrt(SquareLength());
}

// ------------------------------------------------------------------------------------------------
template <typename TReal>
Vector2t<TReal>& Vector2t<TReal>::Normalize() {
	*this /= Length();
	return *this;
}

// ------------------------------------------------------------------------------------------------
template <typename TReal>
const Vector2t<TReal>& Vector2t<TReal>::operator += (const Vector2t& o) {
	x += o.x; y += o.y;
	return *this;
}

// ------------------------------------------------------------------------------------------------
template <typename TReal>
const Vector2t<TReal>& Vector2t<TReal>::operator -= (const Vector2t& o) {
	x -= o.x; y -= o.y;
	return *this;
}

// ------------------------------------------------------------------------------------------------
template <typename TReal>
const Vector2t<TReal>& Vector2t<TReal>::operator *= (TReal f) {
	x *= f; y *= f;
	return *this;
}

// ------------------------------------------------------------------------------------------------
template <typename TReal>
const Vector2t<TReal>& Vector2t<TReal>::operator /= (TReal f) {
	x /= f; y /= f;
	return *this;
}

// ------------------------------------------------------------------------------------------------
template <typename TReal>
TReal Vector2t<TReal>::operator[](unsigned int i) const {
	return *(&x + i);
}

// ------------------------------------------------------------------------------------------------
template <typename TReal>
TReal& Vector2t<TReal>::operator[](unsigned int i) {
	return *(&x + i);
}

// ------------------------------------------------------------------------------------------------
template <typename TReal>
bool Vector2t<TReal>::operator== (const Vector2t& other) const {
	return x == other.x && y == other.y;
}

// ------------------------------------------------------------------------------------------------
template <typename TReal>
bool Vector2t<TReal>::operator!= (const Vector2t& other) const {
	return x != other.x || y != other.y;
}

// ---------------------------------------------------------------------------
template<typename TReal>
bool Vector2t<TReal>::Equal(const Vector2t& other, TReal epsilon) const {
	return
		std::abs(x - other.x) <= epsilon &&
		std::abs(y - other.y) <= epsilon;
}

// ------------------------------------------------------------------------------------------------
template <typename TReal>
Vector2t<TReal>& Vector2t<TReal>::operator= (TReal f) {
	x = y = f;
	return *this;
}

// ------------------------------------------------------------------------------------------------
template <typename TReal>
const Vector2t<TReal> Vector2t<TReal>::SymMul(const Vector2t& o) {
	return Vector2t(x*o.x, y*o.y);
}


// ------------------------------------------------------------------------------------------------
// symmetric addition
template <typename TReal>
inline Vector2t<TReal> operator + (const Vector2t<TReal>& v1, const Vector2t<TReal>& v2) {
	return Vector2t<TReal>(v1.x + v2.x, v1.y + v2.y);
}

// ------------------------------------------------------------------------------------------------
// symmetric subtraction
template <typename TReal>
inline Vector2t<TReal> operator - (const Vector2t<TReal>& v1, const Vector2t<TReal>& v2) {
	return Vector2t<TReal>(v1.x - v2.x, v1.y - v2.y);
}

// ------------------------------------------------------------------------------------------------
// scalar product
template <typename TReal>
inline TReal operator * (const Vector2t<TReal>& v1, const Vector2t<TReal>& v2) {
	return v1.x*v2.x + v1.y*v2.y;
}

// ------------------------------------------------------------------------------------------------
// scalar multiplication
template <typename TReal>
inline Vector2t<TReal> operator * (TReal f, const Vector2t<TReal>& v) {
	return Vector2t<TReal>(f*v.x, f*v.y);
}

// ------------------------------------------------------------------------------------------------
// and the other way around
template <typename TReal>
inline Vector2t<TReal> operator * (const Vector2t<TReal>& v, TReal f) {
	return Vector2t<TReal>(f*v.x, f*v.y);
}

// ------------------------------------------------------------------------------------------------
// scalar division
template <typename TReal>
inline Vector2t<TReal> operator / (const Vector2t<TReal>& v, TReal f) {

	return v * (1 / f);
}

// ------------------------------------------------------------------------------------------------
// vector division
template <typename TReal>
inline Vector2t<TReal> operator / (const Vector2t<TReal>& v, const Vector2t<TReal>& v2) {
	return Vector2t<TReal>(v.x / v2.x, v.y / v2.y);
}

// ------------------------------------------------------------------------------------------------
// vector negation
template <typename TReal>
inline Vector2t<TReal> operator - (const Vector2t<TReal>& v) {
	return Vector2t<TReal>(-v.x, -v.y);
}
