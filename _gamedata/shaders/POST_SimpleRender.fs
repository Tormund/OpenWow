#version 330
#include "lib/fragDeferredRead.glsl"

// Uniform
uniform vec2 gScreenSize;

// Out
out vec4 FragColor;

void main(void)
{
	vec2 pixelXY = gl_FragCoord.xy / gScreenSize;
	
	vec3 WorldPos = getPos(pixelXY);
	vec3 Normal = getNormal(pixelXY);
	vec3 Color = getAlbedo(pixelXY);
	
	FragColor = vec4(Color, 1.0);
}