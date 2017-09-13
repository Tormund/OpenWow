#pragma once

class Bone;
class Model;

struct RibbonSegment
{
	vec3 pos, up, back;
	float len, len0;
};

class RibbonEmitter
{
public:
	void init(File &f, ModelRibbonEmitterDef &mta, uint32_t *globals);
	void setup(int anim, int time);
	void draw();

public:
	Model* model;

private:
	Animated<vec3> color;
	AnimatedShort opacity;
	Animated<float> above;
	Animated<float> below;

	Bone* parent;
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