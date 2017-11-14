#version 330
#include "lib/fragDeferredWrite.glsl"

struct VSOutput
{
	vec3 WorldSpacePos;
	vec3 Color;
};

// Input
in VSOutput VSout;

void main(void)
{
	setMatID(-1.0);
	setPos(VSout.WorldSpacePos);
	setNormal(vec3(0.0, 0.0, 0.0));
	setAlbedo(VSout.Color);
	setSpecParams(vec3(0.0, 0.0, 0.0), 1.0);
};