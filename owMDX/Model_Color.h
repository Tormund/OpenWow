#pragma once

// block G - color defs
struct ModelColorDef
{
	AnimationBlock color;
	AnimationBlock opacity;
};

class ModelColor
{
public:
	void init(File& f, ModelColorDef& mcd, uint32_t * global)
	{
		color.init(mcd.color, f, global);
		opacity.init(mcd.opacity, f, global);
	}

public:
	Animated<vec3> color;
	AnimatedShort opacity;
};