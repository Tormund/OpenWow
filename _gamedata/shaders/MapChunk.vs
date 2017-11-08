#version 330

struct VSOutputType
{
	vec3 WorldSpacePos;
	vec2 TexCoordDetail;
	vec2 TexCoordAlpha;
	vec3 Normal;
	vec3 VertexColorMCCV;
	vec4 VertexColorMCLV;
};

// Vertex attrib
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec2 TexCoordDetail;
layout(location = 2) in vec2 TexCoordAlpha;
layout(location = 3) in vec3 Normal;
layout(location = 4) in vec3 VertexColorMCCV;
layout(location = 5) in vec4 VertexColorMCLV;

// Uniforms
uniform mat4 gProjection;
uniform mat4 gView;
uniform mat4 gWorld;

// Out
out VSOutputType VSInput;

void main(void)
{
	mat4 PVW = gProjection * gView * gWorld;
	gl_Position = PVW * vec4(VertexPosition, 1.0);

	VSInput.WorldSpacePos = (gWorld * vec4(VertexPosition, 1.0)).xyz;
	VSInput.TexCoordDetail = TexCoordDetail;
	VSInput.TexCoordAlpha = TexCoordAlpha;
	VSInput.Normal = (gWorld * vec4(Normal, 0.0)).xyz;
	VSInput.VertexColorMCCV = VertexColorMCCV;
	VSInput.VertexColorMCLV = VertexColorMCLV;
};