#pragma once

class MDX_Part_TextureAnim
{
public:
	void init(File& f, M2TextureTransform& mta, uint32* global);
	void calc(int anim, int time);
	void setup(int anim);

public:
	vec3 tval, rval, sval;

private:
	Animated<vec3> trans, roll, scale;
};