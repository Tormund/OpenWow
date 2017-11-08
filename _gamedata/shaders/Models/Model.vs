#version 330

struct VSOutput
{
	vec3 position;
	vec3 normal;
	vec2 tc;
};

// Vertex attrib
layout(location = 0) in vec3 position;
layout(location = 1) in int blendWeight;
layout(location = 2) in int blendIndices;
layout(location = 3) in vec3 normal;
layout(location = 4) in vec2 tc0;
layout(location = 5) in vec2 tc1;

// Uniforms
uniform mat4 gProjection;
uniform mat4 gView;
uniform mat4 gWorld;

// Bones
uniform bool gIsAnimated;
uniform int gBoneStartIndex;
uniform int gBoneMaxCount;
uniform mat4 gBones[251];

// Billboard
uniform bool gBillboard;

// Texture animating
uniform int gTextureAnimEnable;
uniform mat4 gTextureAnimMatrix;

// Out
out VSOutput VSout;

void main(void)
{
	vec4 newVertex = vec4(0.0f);

	if(gIsAnimated)
	{
		int blendWeights[4];
		blendWeights[0] = blendWeight & 0xFF000000;
		blendWeights[1] = blendWeight & 0x00FF0000;
		blendWeights[2] = blendWeight & 0x0000FF00;
		blendWeights[3] = blendWeight & 0x000000FF;
			
		int blendIndicess[4];
		blendIndicess[0] = blendIndices & 0xFF000000;
		blendIndicess[1] = blendIndices & 0x00FF0000;
		blendIndicess[2] = blendIndices & 0x0000FF00;
		blendIndicess[3] = blendIndices & 0x000000FF;

		for(int i = 0; i < 4; i++)
		{
			if(blendWeights[i] > 0)
			{
				newVertex += gBones[blendIndicess[i]] * vec4(position, 1.0f) * (float(blendWeights[i]) / 255.0f);
			}
		}
	}
	else
	{
		newVertex = vec4(position, 1.0f);
	}
		
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
		gl_Position = gProjection * gView * gWorld * newVertex;
	//}
	
	VSout.position = (gWorld * newVertex).xyz;
	VSout.normal = (gWorld * vec4(normal, 0.0)).xyz;
	
	if (gTextureAnimEnable > 0)
	{
		VSout.tc = (gTextureAnimMatrix * vec4(tc0, 1.0, 1.0)).xy;
	}
	else
	{
		VSout.tc = tc0;
	}
};