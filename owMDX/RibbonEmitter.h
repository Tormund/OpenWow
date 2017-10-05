#pragma once

class MDX_Part_Bone;
class MDX;

struct RibbonSegment
{
	vec3 pos, up, back;
	float len, len0;
};

class RibbonEmitter
{
public:
	void init(File& f, M2Ribbon &mta, uint32 *globals);
	void setup(int anim, int time);
	void draw();

public:
	MDX* model;

private:
	Animated<vec3> color;
	Animated<float, short, ShortToFloat> opacity;
	Animated<float> above;
	Animated<float> below;

	MDX_Part_Bone* parent;
	float f1, f2;

	vec3 pos;

	int manim, mtime;
	float length, seglen;
	int numsegs;

	vec3 tpos;
	vec4 tcolor;
	float tabove, tbelow;

	Texture* texture;

	list<RibbonSegment> segs;
};