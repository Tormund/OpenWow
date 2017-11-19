#version 330
#include "lib/fragDeferredWrite.glsl"

struct VSOutput
{
	vec3 WorldSpacePos;
	vec3 TexCoord;
	vec3 Normal;
};

// Input
in VSOutput VSout;

// Uniforms
uniform sampler2D gColorMap;
uniform sampler2D gSpecularMap;

uniform float gShallowAlpha;
uniform float gDeepAlpha;
uniform vec3 gColorLight;
uniform vec3 gColorDark;

void main(void)
{
	float alpha = texture(gColorMap, VSout.TexCoord.xy).a;

	vec4 resultColor = vec4(gColorLight, (1.0 - VSout.TexCoord.z) * gShallowAlpha) + vec4(gColorDark, VSout.TexCoord.z * gDeepAlpha);
	
	resultColor *= (1.0 - alpha);
	resultColor += vec4(1,1,1,1) * alpha;
	
	setMatID(1.0);
	setPos(VSout.WorldSpacePos);
	setNormal(normalize(VSout.Normal));
	setAlbedo4(resultColor);
	setSpecParams(texture(gSpecularMap, VSout.TexCoord.xy).rgb, 1.0);
};