#version 330
#include "lib/fragDeferredRead.glsl"

// Uniform
uniform vec2 gScreenSize;
uniform vec3 gCameraPosition;

uniform vec3 gAmbientColor;
uniform float gAmbientIntensitive;

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
	
	if(materialID >= 0)
	{
		FragColor = vec4(Color.rgb * gAmbientColor, 1.0);
	}
	else
	{
		FragColor = vec4(Color.rgb, 1.0);
	}
}