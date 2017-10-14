#version 330
#include "lib/fragDeferredWrite.glsl"

struct VSOutput
{
	vec3 WorldSpacePos;
	vec2 TexCoordDetail;
	vec2 TexCoordAlpha;
	vec3 Normal;
	vec3 VertexColorMCCV;
	vec4 VertexColorMCLV;
};

// In
in VSOutput VSout;

// Layers
uniform int gLayersCount;

// Diffuses
uniform sampler2D gColorMap0;
uniform sampler2D gColorMap1;
uniform sampler2D gColorMap2;
uniform sampler2D gColorMap3;

// Alpha Textures + Shadow Texture
uniform sampler2D gBlend;

// Speculars textures
uniform sampler2D gSpecularMap0;
uniform sampler2D gSpecularMap1;
uniform sampler2D gSpecularMap2;
uniform sampler2D gSpecularMap3;

// Shadow Params
uniform bool gShadowMapExists;
uniform vec3 gShadowColor;

// MCCV & MCLV Exists
uniform bool gMCCVExists;
uniform bool gMCLVExists;

void main()
{
	vec3 resultColor = vec3(0.0, 0.0, 0.0);
	float alphaSumma = 0.0;
	float alphaCurrent;

	float specularAlpha = 0.0;

	if (gLayersCount > 1)
	{
		alphaCurrent = texture(gBlend, VSout.TexCoordAlpha).r;
		alphaSumma += alphaCurrent;
		resultColor += texture(gColorMap1, VSout.TexCoordDetail).rgb * alphaCurrent;
		specularAlpha += alphaCurrent * texture(gSpecularMap1, VSout.TexCoordDetail).a;
	}

	if (gLayersCount > 2)
	{
		alphaCurrent = texture(gBlend, VSout.TexCoordAlpha).g;
		alphaSumma += alphaCurrent;
		resultColor += texture(gColorMap2, VSout.TexCoordDetail).rgb * alphaCurrent;
		specularAlpha += alphaCurrent * texture(gSpecularMap2, VSout.TexCoordDetail).a;
	}

	if (gLayersCount > 3)
	{
		alphaCurrent = texture(gBlend, VSout.TexCoordAlpha).b;
		alphaSumma += alphaCurrent;
		resultColor += texture(gColorMap3, VSout.TexCoordDetail).rgb * alphaCurrent;
		specularAlpha += alphaCurrent * texture(gSpecularMap3, VSout.TexCoordDetail).a;
	}

	resultColor += texture(gColorMap0, VSout.TexCoordDetail).rgb * (1.0 - alphaSumma);
	specularAlpha += texture(gSpecularMap0, VSout.TexCoordDetail).a * (1.0 - alphaSumma);

	if (gShadowMapExists)
	{
		alphaCurrent = texture(gBlend, VSout.TexCoordAlpha).a;
		resultColor = resultColor * (1.0 - alphaCurrent) + gShadowColor * alphaCurrent;
	}

	if (gMCLVExists)
	{
		resultColor += (VSout.VertexColorMCLV.rgb * specularAlpha);
	}

	if (gMCCVExists)
	{
		resultColor *= VSout.VertexColorMCCV;
	}

	//
	setMatID(1.0);
	setPos(VSout.WorldSpacePos);
	setNormal(normalize(VSout.Normal));
	setAlbedo(resultColor);
	setSpecParams(texture(gSpecularMap0, VSout.TexCoordDetail).rgb, 1.0);
};
