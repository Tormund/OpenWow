#version 330

// Input
in vec2 OutTextCoord;

// Uniforms
uniform sampler2D gFontTexture;
uniform vec3 gFontColor;

// Out
out vec4 FragColor;

void main(void)
{
	FragColor = vec4(gFontColor, texture(gFontTexture, OutTextCoord).a);
};