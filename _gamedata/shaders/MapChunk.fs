#version 330
#include "lib/fragDeferredWrite.glsl"

// In put
in struct
{
	vec3 WorldSpacePos;
	vec3 Normal;
	vec4 VertexColorMCCV;
	vec4 VertexColorMCLV;
	vec2 TexCoordDetail;
	vec2 TexCoordAlpha;
} VSInput;

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
		float alphaCurrent = texture(gBlend, VSInput.TexCoordAlpha).a;
		resultColor = resultColor * (1.0 - alphaCurrent) + gShadowColor * alphaCurrent;
	}

	if (gMCLVExists)
	{
		vec3 orig = resultColor;
		//resultColor *= VSInput.VertexColorMCLV.rgb;
		//resultColor = (vec3(1,1,1) - resultColor) * orig;
		//resultColor = (vec3(1,1,1) + VSInput.VertexColorMCLV.rgb) / 2.0;
		
		//3
		//vec3 col = mix(vec3(1,1,1), VSInput.VertexColorMCLV.rgb, VSInput.VertexColorMCLV.a);
		//resultColor *= col;
		resultColor += 0.5 * VSInput.VertexColorMCLV.rgb * VSInput.VertexColorMCLV.a;
	}

	if (gMCCVExists)
	{
		resultColor *= VSInput.VertexColorMCCV.rgb;
	}

	//
	setMatID(1.0);
	setPos(VSInput.WorldSpacePos);
	setNormal(VSInput.Normal);
	setAlbedo(resultColor.rgb);
	setSpecParams(resultSpecular.rgb, 16.0);
};
