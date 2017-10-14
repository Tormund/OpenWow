#version 330

struct VSOutput
{
	vec3 WorldSpacePos;
	vec2 TexCoord;
	vec3 Normal;
	//vec3 Color;
};

// Vertex attrib
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 Normal;
//layout(location = 3) in vec3 Color;

// Input

// Uniforms
uniform mat4 gProjection;
uniform mat4 gView;
uniform mat4 gWorld;

// Out
out VSOutput VSout;

void main(void)
{
	mat4 PVW = gProjection * gView * gWorld;
	gl_Position = PVW * vec4(VertexPosition, 1.0);

	VSout.WorldSpacePos = (gWorld * vec4(VertexPosition, 1.0)).xyz;
	VSout.TexCoord = TexCoord;
	VSout.Normal = (gWorld * vec4(Normal, 0.0)).xyz;
	//VSout.Color = Color;
};