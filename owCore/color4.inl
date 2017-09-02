#pragma once

#include "color4.h"

template <typename T>
FORCE_INLINE const Color4t<T>& Color4t<T>::operator += (const Color4t<T>& o) {
	r += o.r; g += o.g; b += o.b; a += o.a;
	return *this;
}

template <typename T>
FORCE_INLINE const Color4t<T>& Color4t<T>::operator -= (const Color4t<T>& o) {
	r -= o.r; g -= o.g; b -= o.b; a -= o.a;
	return *this;
}

template <typename T>
FORCE_INLINE const Color4t<T>& Color4t<T>::operator *= (T f) {
	r *= f; g *= f; b *= f; a *= f;
	return *this;
}

template <typename T>
FORCE_INLINE const Color4t<T>& Color4t<T>::operator /= (T f) {
	r /= f; g /= f; b /= f; a /= f;
	return *this;
}

template <typename T>
FORCE_INLINE T Color4t<T>::operator [] (unsigned int i) const {
	return *(&r + i);
}

template <typename T>
FORCE_INLINE T& Color4t<T>::operator [] (unsigned int i) {
	return *(&r + i);
}

template <typename T>
FORCE_INLINE bool Color4t<T>::operator == (const Color4t<T>& other) const {
	return r == other.r && g == other.g && b == other.b && a == other.a;
}

template <typename T>
FORCE_INLINE bool Color4t<T>::operator != (const Color4t<T>& other) const {
	return r != other.r || g != other.g || b != other.b || a != other.a;
}

template <typename T>
FORCE_INLINE bool Color4t<T>::operator < (const Color4t<T>& other) const {
	return r < other.r || (r == other.r && (g < other.g || (g == other.g && (b < other.b || (b == other.b && (a < other.a))))));
}

template <typename T>
FORCE_INLINE Color4t<T> operator + (const Color4t<T>& v1, const Color4t<T>& v2) {
	return Color4t<T>(v1.r + v2.r, v1.g + v2.g, v1.b + v2.b, v1.a + v2.a);
}

template <typename T>
FORCE_INLINE Color4t<T> operator - (const Color4t<T>& v1, const Color4t<T>& v2) {
	return Color4t<T>(v1.r - v2.r, v1.g - v2.g, v1.b - v2.b, v1.a - v2.a);
}

template <typename T>
FORCE_INLINE Color4t<T> operator * (const Color4t<T>& v1, const Color4t<T>& v2) {
	return Color4t<T>(v1.r * v2.r, v1.g * v2.g, v1.b * v2.b, v1.a * v2.a);
}

template <typename T>
FORCE_INLINE Color4t<T> operator / (const Color4t<T>& v1, const Color4t<T>& v2) {
	return Color4t<T>(v1.r / v2.r, v1.g / v2.g, v1.b / v2.b, v1.a / v2.a);
}

template <typename T>
FORCE_INLINE Color4t<T> operator * (T f, const Color4t<T>& v) {
	return Color4t<T>(f*v.r, f*v.g, f*v.b, f*v.a);
}

template <typename T>
FORCE_INLINE Color4t<T> operator * (const Color4t<T>& v, T f) {
	return Color4t<T>(f*v.r, f*v.g, f*v.b, f*v.a);
}

template <typename T>
FORCE_INLINE Color4t<T> operator / (const Color4t<T>& v, T f) {
	return v * (1 / f);
}

template <typename T>
FORCE_INLINE Color4t<T> operator / (T f, const Color4t<T>& v) {
	return Color4t<T>(f, f, f, f) / v;
}

template <typename T>
FORCE_INLINE Color4t<T> operator + (const Color4t<T>& v, T f) {
	return Color4t<T>(f + v.r, f + v.g, f + v.b, f + v.a);
}

template <typename T>
FORCE_INLINE Color4t<T> operator - (const Color4t<T>& v, T f) {
	return Color4t<T>(v.r - f, v.g - f, v.b - f, v.a - f);
}

template <typename T>
FORCE_INLINE Color4t<T> operator + (T f, const Color4t<T>& v) {
	return Color4t<T>(f + v.r, f + v.g, f + v.b, f + v.a);
}

template <typename T>
FORCE_INLINE Color4t<T> operator - (T f, const Color4t<T>& v) {
	return Color4t<T>(f - v.r, f - v.g, f - v.b, f - v.a);
}


template <typename T>
inline bool Color4t<T>::IsBlack() const {
	static const T epsilon = 10e-3f;
	return std::fabs(r) < epsilon && std::fabs(g) < epsilon && std::fabs(b) < epsilon;
}
