#pragma once

class MDX_Part_Color
{
public:
	void init(File& f, M2Color& mcd, uint32_t* global);

public:
	Animated<vec3> color;
	Animated<float, short, ShortToFloat> opacity;
};