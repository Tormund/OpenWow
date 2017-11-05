#pragma once

class Rect
{
public:
	// Zero constructor
	Rect() : p0(vec2(0, 0)), p1(vec2(0, 0)) {}

	// Constructor
	Rect(float _x0, float _y0, float _x1, float _y1) : p0(vec2(_x0, _y0)), p1(vec2(_x1, _y1)) {}

	// Vector constructor
	Rect(vec2 _p0, vec2 _p1) : p0(_p0), p1(_p1) {}

	// Other
	Rect& operator=(const Rect& _other)
	{
		p0 = _other.p0;
		p1 = _other.p1;
		return *this;
	}

	vec2 p0;
	vec2 p1;
};

const Rect RECT_ZERO = Rect();
const Rect RECT_ONE = Rect(0.0f, 0.0f, 1.0f, 1.0f);
