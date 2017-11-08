#version 330
#include "lib/fragDeferredWrite.glsl"

struct VSOutputType
{
	vec3 WorldSpacePos;
	vec2 TexCoordDetail;
	vec2 TexCoordAlpha;
	vec3 Normal;
	vec3 VertexColorMCCV;
	vec4 VertexColorMCLV;
};

// In
in VSOutputType VSInput;

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
	float alphaSumma = 0.0;
	float specularAlpha = 0.0;
	
	vec3 layersColor = vec3(0);
	if (gLayersCount > 1)
	{
		float alphaCurrent = texture(gBlend, VSInput.TexCoordAlpha).r;
		alphaSumma += alphaCurrent;
		layersColor += texture(gColorMap1, VSInput.TexCoordDetail).rgb * alphaCurrent;
		specularAlpha += alphaCurrent * texture(gSpecularMap1, VSInput.TexCoordDetail).a;
	}

	if (gLayersCount > 2)
	{
		float alphaCurrent = texture(gBlend, VSInput.TexCoordAlpha).g;
		alphaSumma += alphaCurrent;
		layersColor += texture(gColorMap2, VSInput.TexCoordDetail).rgb * alphaCurrent;
		specularAlpha += alphaCurrent * texture(gSpecularMap2, VSInput.TexCoordDetail).a;
	}

	if (gLayersCount > 3)
	{
		float alphaCurrent = texture(gBlend, VSInput.TexCoordAlpha).b;
		alphaSumma += alphaCurrent;
		layersColor += texture(gColorMap3, VSInput.TexCoordDetail).rgb * alphaCurrent;
		specularAlpha += alphaCurrent * texture(gSpecularMap3, VSInput.TexCoordDetail).a;
	}

	vec3 resultColor = texture(gColorMap0, VSInput.TexCoordDetail).rgb * (1.0 - alphaSumma) + layersColor;
	
	//resultColor += 
	//specularAlpha += texture(gSpecularMap0, VSInput.TexCoordDetail).a * (1.0 - alphaSumma);

	if (gShadowMapExists)
	{
		//float alphaCurrent = texture(gBlend, VSInput.TexCoordAlpha).a;
		//resultColor = resultColor * (1.0 - alphaCurrent) + gShadowColor * alphaCurrent;
	}

	if (gMCLVExists)
	{
		resultColor += (VSInput.VertexColorMCLV.rgb * specularAlpha);
	}

	if (gMCCVExists)
	{
		resultColor *= VSInput.VertexColorMCCV;
	}

	//
	setMatID(1.0);
	setPos(VSInput.WorldSpacePos);
	setNormal(normalize(VSInput.Normal));
	setAlbedo(resultColor);
	setSpecParams(texture(gSpecularMap0, VSInput.TexCoordDetail).rgb, 1.0);
};
