#version 330

in vec2 OutTextCoord;

uniform sampler2D gTexture;

out vec4 FragColor;

void main(void)
{
	FragColor = texture(gTexture, OutTextCoord);
}
