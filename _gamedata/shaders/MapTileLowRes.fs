#version 330
#include "lib/fragDeferredWrite.glsl"

struct VSOutput
{
	vec3 WorldSpacePos;
};

// Input
in VSOutput VSout;

// Uniforms
uniform vec3 gColor;

void main(void)
{
	setMatID(-1.0);
	setPos(VSout.WorldSpacePos);
	setNormal(vec3(0.0, 0.0, 0.0));
	setAlbedo(gColor);
	setSpecParams(vec3(0.0, 0.0, 0.0), 0.0);
};