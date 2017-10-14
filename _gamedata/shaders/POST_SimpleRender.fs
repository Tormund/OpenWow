#version 330
#include "Postprocess_Common.glsl"

// Out
out vec4 FragColor;

void main(void)
{
	vec2 pixelXY = gl_FragCoord.xy / gScreenSize;
	vec3 WorldPos = texture(gbuffer.gWorldSpacePosMap, pixelXY).xyz;
	vec4 Color = texture(gbuffer.gDiffuseMap, pixelXY);
	vec3 Normal = normalize(texture(gbuffer.gNormalMap, pixelXY).xyz);

	FragColor = Color; //vec4(Color.rgb, 1.0);
}