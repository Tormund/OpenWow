#pragma once

#include "pushpack.h"

template <typename T>
class Color4t {
public:
	Color4t() : r(), g(), b(), a() { }
	Color4t(T _r, T _g, T _b, T _a) : r(_r), g(_g), b(_b), a(_a) { }
	explicit Color4t(T _r) : r(_r), g(_r), b(_r), a(_r) { }
	Color4t(const Color4t& o) : r(o.r), g(o.g), b(o.b), a(o.a) { }

public:
	const Color4t& operator += (const Color4t& o);
	const Color4t& operator -= (const Color4t& o);
	const Color4t& operator *= (T f);
	const Color4t& operator /= (T f);

public:
	bool operator == (const Color4t& other) const;
	bool operator != (const Color4t& other) const;
	bool operator <  (const Color4t& other) const;

	// color tuple access, rgba order
	inline T operator[](unsigned int i) const;
	inline T& operator[](unsigned int i);

	// check whether a color is (close to) black
	inline bool IsBlack() const;

public:
	T r, g, b, a;
};

typedef Color4t<float> Color4D;

#include "poppack.h"
