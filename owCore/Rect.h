#pragma once

template <typename T>
class Rect {
public:
	// Zero constructor
	Rect() : p0(vec2(0, 0)), p1(vec2(0, 0)) { }

	// Constructor
	Rect(T _x0, T _y0, T _x1, T _y1) : p0(vec2(_x0, _y0)), p1(vec2(_x1, _y1)) {	}

	// Vector constructor
	Rect(vec2 _p0, vec2 _p1) : p0(_p0), p1(_p1) { }

	// Other
	template <class T2>
	Rect& operator=(const Rect<T2>& _other) {
		p0 = _other.p0;
		p1 = _other.p1;
		return *this;
	}

	vec2 p0;
	vec2 p1;
};

const Rect<int> ZERO_RECT = Rect<int>();
