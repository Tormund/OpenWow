#pragma once

#include <cmath>

#include "pushpack.h"

template <typename T>
class Vector2t {
public:
	Vector2t() : x(), y() { }
	Vector2t(T _x, T _y) : x(_x), y(_y) { }
	explicit Vector2t(T _xyz) : x(_xyz), y(_xyz) { }
	Vector2t(const Vector2t& o) : x(o.x), y(o.y) { }

public:
	void Set(T pX, T pY);
	T SquareLength() const;
	T Length() const;
	Vector2t& Normalize();

public:
	const Vector2t& operator += (const Vector2t& o);
	const Vector2t& operator -= (const Vector2t& o);
	const Vector2t& operator *= (T f);
	const Vector2t& operator /= (T f);

	T operator[](unsigned int i) const;
	T& operator[](unsigned int i);

	bool operator== (const Vector2t& other) const;
	bool operator!= (const Vector2t& other) const;

	bool Equal(const Vector2t& other, T epsilon = 1e-6) const;

	Vector2t& operator= (T f);
	const Vector2t SymMul(const Vector2t& o);

	template <typename TOther>
	operator Vector2t<TOther>() const;

	T x, y;
} PACK_STRUCT;

typedef Vector2t<float> Vector2D;

#include "poppack.h"

