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
uniform mat4 gProjection;
uniform mat4 gView;
uniform mat4 gWorld;

// Billboard
uniform bool gBillboard;

// Texture animating
uniform int gTextureAnimEnable;
uniform mat4 gTextureAnimMatrix;

// Out
out VSOutput VSout;

void main(void)
{
	/*if (gBillboard)
	{
		mat4 VW = gView * gWorld;
			
		// Column 0:
	    VW[0][0] = 1;
		VW[0][1] = 0;
		VW[0][2] = 0;

		// Column 1:
		VW[1][0] = 0;
	    VW[1][1] = 1;
		VW[1][2] = 0;

		// Column 2:
		VW[2][0] = 0;
		VW[2][1] = 0;
	    VW[2][2] = 1;
		
		gl_Position = gProjection * VW * vec4(position, 1.0);
	}
	else*/
	//{
		gl_Position = gProjection * gView * gWorld * vec4(position, 1.0);
	//}
	
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