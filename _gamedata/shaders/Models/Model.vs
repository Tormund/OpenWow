#version 330

struct VSOutput
{
	vec3 position;
	vec3 normal;
	vec2 tc;
};

// Vertex attrib
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tc;


// Uniforms
uniform mat4 gPV;
uniform mat4 gWorld;

uniform int gTextureAnimEnable;
uniform mat4 gTextureAnimMatrix;

// Out
out VSOutput VSout;

void main(void)
{
	gl_Position = gPV * gWorld * vec4(position, 1.0);

	VSout.position = (gWorld * vec4(position, 1.0)).xyz;
	VSout.normal = (gWorld * vec4(normal, 0.0)).xyz;
	
	if (gTextureAnimEnable > 0)
	{
		VSout.tc = (gTextureAnimMatrix * vec4(tc, 1.0, 1.0)).xy;
	}
	else
	{
		VSout.tc = tc;
	}
};