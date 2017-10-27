#version 330
#include "lib/fragDeferredWrite.glsl"

struct VSOutput
{
	vec3 WorldSpacePos;
	vec2 TexCoord;
	vec3 Normal;
	//vec3 Color;
};

// Input
in VSOutput VSout;

// Uniforms
uniform sampler2D gColorMap;
uniform sampler2D gSpecularMap;

uniform bool gHasMOCV;
uniform vec3 gDiffuseColor;

void main(void)
{
	setMatID(1.0);
	setPos(VSout.WorldSpacePos);
	setNormal(normalize(VSout.Normal));
	setAlbedo(texture(gColorMap, VSout.TexCoord).rgb);
	setSpecParams(texture(gSpecularMap, VSout.TexCoord).rgb, 1.0);
	
	
	fragData2.a = texture(gColorMap, VSout.TexCoord).a;
};