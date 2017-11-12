#version 330

// Vertex attrib
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec2 TexCoord;

// Input

// Uniforms
uniform mat4 gProjection;
uniform vec2 gOffset;

// Output
out vec2 OutTextCoord;

void main(void)
{
	gl_Position = gProjection * vec4(VertexPosition + vec3(gOffset, 0.0), 1.0);

	OutTextCoord = TexCoord;
};
