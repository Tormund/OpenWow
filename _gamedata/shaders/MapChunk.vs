#version 330

// Vertex attrib
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec4 VertexColorMCCV;
layout(location = 3) in vec4 VertexColorMCLV;
layout(location = 4) in vec2 TexCoordDetail;
layout(location = 5) in vec2 TexCoordAlpha;

// Output
out struct
{
	vec3 WorldSpacePos;
	vec3 Normal;
	vec4 VertexColorMCCV;
	vec4 VertexColorMCLV;
	vec2 TexCoordDetail;
	vec2 TexCoordAlpha;
} VSInput;

// Uniforms
uniform mat4 gProjection;
uniform mat4 gView;
uniform mat4 gWorld;

void main(void)
{
	mat4 PVW = gProjection * gView;
	gl_Position = PVW * vec4(VertexPosition, 1.0);

	VSInput.WorldSpacePos   = VertexPosition;
	VSInput.Normal          = Normal;
	VSInput.VertexColorMCCV = VertexColorMCCV * 2;				
	VSInput.VertexColorMCLV = VertexColorMCLV;
	VSInput.TexCoordDetail  = TexCoordDetail;
	VSInput.TexCoordAlpha   = TexCoordAlpha;
};