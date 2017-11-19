/*struct VSOutputType
{
	vec3 WorldSpacePos;
	vec2 TexCoordDetail;
	vec2 TexCoordAlpha;
	vec3 Normal;
	vec3 VertexColorMCCV;
	vec4 VertexColorMCLV;
};

// Vertex attrib
layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

// Input
in VSOutputType VSInput[3];

// Out
out VSOutputType VSOutput;

void main(void)
{
  for(int i = 0; i < gl_in.length(); i++)
  {
     // copy attributes
    gl_Position = gl_in[i].gl_Position;
	
    VSOutput.WorldSpacePos   = VSInput[i].WorldSpacePos;
	VSOutput.TexCoordDetail  = VSInput[i].TexCoordDetail;
	VSOutput.TexCoordAlpha   = VSInput[i].TexCoordAlpha;
	VSOutput.Normal          = VSInput[i].Normal;
	VSOutput.VertexColorMCCV = VSInput[i].VertexColorMCCV;
	VSOutput.VertexColorMCLV = VSInput[i].VertexColorMCLV;
 
    // done with the vertex
    EmitVertex();
  }
};*/