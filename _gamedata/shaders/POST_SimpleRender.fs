#version 330
#include "lib/fragDeferredRead.glsl"

// Uniform
uniform vec2 gScreenSize;
uniform vec3 gCameraPosition;

float cameraDistance;
int materialID;

// Out
out vec4 FragColor;

void main(void)
{
	vec2 pixelXY = gl_FragCoord.xy / gScreenSize;
	
	materialID = int(getMatID(pixelXY));
	vec3 WorldPos = getPos(pixelXY);
	vec3 Normal = getNormal(pixelXY);
	vec4 Color = getAlbedo4(pixelXY);
	
	FragColor = Color;
}