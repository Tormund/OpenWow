#version 330
#include "../lib/fragDeferredWrite.glsl"

struct VSOutput
{
	vec3 position;
	vec3 normal;
	vec2 tc;
};

// Input
in VSOutput VSout;

// Colors
uniform vec4 gColor;

// Textures
uniform sampler2D gDiffuseTexture;
uniform sampler2D gSpecularTexture;

// Alpha & Blend
uniform float gAlpha;
uniform int gBlendMode;

uniform float gTextureWeight;



void main(void)
{
	vec4 resultColor;
	
	resultColor = texture(gDiffuseTexture, VSout.tc);
	resultColor.a *= gTextureWeight;
	resultColor.rgb *= (gColor.rgb * gColor.a);
	resultColor.rgb *= 2.0;
	
	if (gBlendMode == 0) // Opacity
	{
		resultColor.a = 1.0;
		//if (resultColor.a < (128.0 / 255.0))
		//	discard;
	}
	else if(gBlendMode == 1)
	{
		if (resultColor.a < (128.0 / 255.0))
			discard;
	}
	else 
	{
		if (resultColor.a < (1.0 / 255.0))
			discard;
	}
	
	setMatID(1.0);
	setPos(VSout.position);
	setNormal(normalize(VSout.normal));
	setAlbedo4(resultColor);
	setSpecParams(texture(gSpecularTexture, VSout.tc).rgb, 1.0);	
};