#pragma once

struct Color
{
	Color()
	{
		red = 0.0f;
		green = 0.0f;
		blue = 0.0f;
		alpha = 0.0f;
	}

    Color(vec3 _vec)
    {
        red = _vec.x;
        green = _vec.y;
        blue = _vec.z;
        alpha = 1.0f;
    }

	Color(float _Red, float _Green, float _Blue)
	{
		red = _Red;
		green = _Green;
		blue = _Blue;
		alpha = 1.0f;
	}

	Color(float _Red, float _Green, float _Blue, float _Alpha)
	{
		red = _Red;
		green = _Green;
		blue = _Blue;
		alpha = _Alpha;
	}

	Color& operator=(const Color& _color)
	{
		red = _color.red;
		green = _color.green;
		blue = _color.blue;
		alpha = _color.alpha;

		return *this;
	}

	bool operator==(const Color& _other) const
	{
		return red == _other.red && green == _other.green && blue == _other.blue && alpha == _other.alpha;
	}
	bool operator!=(const Color& _other) const
	{
		return red != _other.red || green != _other.green || blue != _other.blue || alpha != _other.alpha;
	}

	Color& Alpha(float _value)
	{
		alpha = _value;
		return *this;
	}

	Color Alpha(float _value) const
	{
		return Color(*this).Alpha(_value);
	}

	float red, green, blue, alpha;
};

const Color COLOR_EMPTY = Color(0.0f, 0.0f, 0.0f, 0.0f);

const Color COLOR_BLACK = Color(0.0f, 0.0f, 0.0f, 1.0f);
const Color COLOR_DARKGRAY = Color(0.3f, 0.3f, 0.3f, 1.0f);
const Color COLOR_GRAY = Color(0.5f, 0.5f, 0.5f, 1.0f);
const Color COLOR_LIGHTGRAY = Color(0.7f, 0.7f, 0.7f, 1.0f);
const Color COLOR_WHITE = Color(1.0f, 1.0f, 1.0f, 1.0f);

const Color COLOR_RED = Color(1.0f, 0.0f, 0.0f, 1.0f);
const Color COLOR_GREEN = Color(0.0f, 1.0f, 0.0f, 1.0f);
const Color COLOR_BLUE = Color(0.0f, 0.0f, 1.0f, 1.0f);

const Color COLOR_LIGHTBLUE = Color(0.0f, 1.0f, 1.0f, 1.0f);
const Color COLOR_PINK = Color(1.0f, 0.0f, 1.0f, 1.0f);
const Color COLOR_YELLOW = Color(1.0f, 1.0f, 0.0f, 1.0f);
