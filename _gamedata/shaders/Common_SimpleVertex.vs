#version 330

uniform mat4 gProjection;
uniform mat4 gView;
uniform mat4 gWorld;

layout(location = 0) in vec2 VertexPosition;

void main(void)
{
	//mat4 PVW = gProjection * gView * gWorld;
	gl_Position = vec4(VertexPosition, 0.0, 1.0);
}
