#version 330

// Vertex attrib
layout(location = 0) in vec2 VertexPosition;

// Input

// Uniforms
uniform mat4 gProjection;

// Output

void main(void)
{
	gl_Position = gProjection * vec4(VertexPosition, 1.0, 1.0);
};
