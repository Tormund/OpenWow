#pragma once

struct Color {
	Color() {
		red = 0.0;
		green = 0.0;
		blue = 0.0;
		alpha = 0.0;
	}

	Color(double _Red, double _Green, double _Blue) {
		red = _Red;
		green = _Green;
		blue = _Blue;
		alpha = 1.0;
	}

	Color(double _Red, double _Green, double _Blue, double _Alpha) {
		red = _Red;
		green = _Green;
		blue = _Blue;
		alpha = _Alpha;
	}

	Color& operator=(const Color& _color) {
		red = _color.red;
		green = _color.green;
		blue = _color.blue;
		alpha = _color.alpha;

		return *this;
	}

	bool operator==(const Color& _other) const {
		return red == _other.red && green == _other.green && blue == _other.blue && alpha == _other.alpha;
	}
	bool operator!=(const Color& _other) const {
		return red != _other.red || green != _other.green || blue != _other.blue || alpha != _other.alpha;
	}

	Color& Alpha(double _value) {
		alpha = _value;
		return *this;
	}

	Color Alpha(double _value) const {
		return Color(*this).Alpha(_value);
	}

	double red, green, blue, alpha;
};

const Color COLOR_EMPTY = Color(0.0, 0.0, 0.0, 0.0);

const Color COLOR_BLACK = Color(0.0, 0.0, 0.0, 1.0);
const Color COLOR_DARKGRAY = Color(0.3, 0.3, 0.3, 1.0);
const Color COLOR_GRAY = Color(0.5, 0.5, 0.5, 1.0);
const Color COLOR_LIGHTGRAY = Color(0.7, 0.7, 0.7, 1.0);
const Color COLOR_WHITE = Color(1.0, 1.0, 1.0, 1.0);

const Color COLOR_RED = Color(1.0, 0.0, 0.0, 1.0);
const Color COLOR_GREEN = Color(0.0, 1.0, 0.0, 1.0);
const Color COLOR_BLUE = Color(0.0, 0.0, 1.0, 1.0);

const Color COLOR_LIGHTBLUE = Color(0.0, 1.0, 1.0, 1.0);
const Color COLOR_PINK = Color(1.0, 0.0, 1.0, 1.0);
const Color COLOR_YELLOW = Color(1.0, 1.0, 0.0, 1.0);
