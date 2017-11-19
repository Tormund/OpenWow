#version 330
#include "lib/fragDeferredRead.glsl"

// Uniform
uniform vec2 gScreenSize;
uniform vec3 gCameraPosition;

uniform float gFogDistance;
uniform float gFogModifier;
uniform vec3  gFogColor;

float cameraDistance;
float cameraDistance2D;
int materialID;

// Out
out vec4 FragColor;

vec4 applyFog() 
{
    vec4 color = vec4(0.0);
	float fogStart = gFogModifier * gFogDistance;
	float fogFactor = (gFogDistance - cameraDistance2D) / (gFogDistance - fogStart);
	fogFactor = 1.0 - clamp(fogFactor, 0.0, 1.0);

	if (materialID >=0 && materialID <= 2) 
	{
		color.rgb = gFogColor.rgb * fogFactor;
		color.a = fogFactor;
	}

	// After camera end
	if (cameraDistance > gFogDistance) 
	{
		color.rgb = gFogColor.rgb;
		color.a = 1.0;
	}

	// Ensure certain blending mode pixels fade out as fog increases.
	if (materialID >= 2 && materialID < 6) 
	{
		color.a *= 1.0 - fogFactor;
	}

	return color;
}

void main(void)
{
	vec2 pixelXY = gl_FragCoord.xy / gScreenSize;
	
	materialID = int(getMatID(pixelXY));
	vec3 WorldPos = getPos(pixelXY);
	vec3 Normal = getNormal(pixelXY);
	vec4 Color = getAlbedo4(pixelXY);
	
	cameraDistance = distance(WorldPos, gCameraPosition);
	cameraDistance2D = distance(WorldPos.xz, gCameraPosition.xz);
	
	FragColor = applyFog();
}