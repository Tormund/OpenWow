#version 330
#include "lib/fragDeferredWrite.glsl"

struct VSOutputType
{
	vec3 WorldSpacePos;
	vec3 Normal;
	vec4 VertexColorMCCV;
	vec4 VertexColorMCLV;
	vec2 TexCoordDetail;
	vec2 TexCoordAlpha;
};

// In
in VSOutputType VSInput;

// Layers
uniform int gLayersCount;

// Diffuses
uniform sampler2D gColorMap[4];

// Alpha Textures + Shadow Texture
uniform sampler2D gBlend;

// Speculars textures
uniform sampler2D gSpecularMap[4];

// Shadow Params
uniform bool gShadowMapExists;
uniform vec3 gShadowColor;

// MCCV & MCLV Exists
uniform bool gMCCVExists;
uniform bool gMCLVExists;

void main()
{
	float alphaSumma = 0.0;

	vec3 layersColor = vec3(0);
	vec4 layersSpecular = vec4(0);
	
	for(int i = 1; i < gLayersCount; i++)
	{
		float alphaCurrent = texture(gBlend, VSInput.TexCoordAlpha)[i - 1];
		alphaSumma += alphaCurrent;
		layersColor += texture(gColorMap[i], VSInput.TexCoordDetail).rgb * alphaCurrent;
		layersSpecular += texture(gSpecularMap[i], VSInput.TexCoordDetail) * alphaCurrent;
	}
	
	vec3 resultColor = texture(gColorMap[0], VSInput.TexCoordDetail).rgb * (1.0 - alphaSumma) + layersColor;
	vec4 resultSpecular = texture(gSpecularMap[0], VSInput.TexCoordDetail) * (1.0 - alphaSumma) + layersSpecular;
	
	if (gShadowMapExists)
	{
		//float alphaCurrent = texture(gBlend, VSInput.TexCoordAlpha).a;
		//resultColor = resultColor * (1.0 - alphaCurrent) + gShadowColor * alphaCurrent;
	}

	if (gMCLVExists)
	{
		//resultColor += (VSInput.VertexColorMCLV.rgb * resultSpecular.a);
	}

	if (gMCCVExists)
	{
		//resultColor *= VSInput.VertexColorMCCV.rgb;
	}

	//
	setMatID(1.0);
	setPos(VSInput.WorldSpacePos);
	setNormal(normalize(VSInput.Normal));
	setAlbedo(resultColor.rgb);
	setSpecParams(resultSpecular.rgb, 1.0);
};
