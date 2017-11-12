#version 330
#include "lib/fragDeferredWrite.glsl"

struct VSOutput
{
	vec3 WorldSpacePos;
};

// Input
in VSOutput VSout;

// Uniforms
uniform vec4 gColor;

void main(void)
{
	setMatID(0.0);
	setPos(VSout.WorldSpacePos);
	setNormal(vec3(0.0, 0.0, 0.0));
	setAlbedo4(gColor);
	setSpecParams(gColor.rgb, gColor.a);
};