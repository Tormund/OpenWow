struct GBuffer
{
	sampler2D gWorldSpacePosMap;     // GL_COLOR_ATTACHMENT0
	sampler2D gNormalMap;		     // GL_COLOR_ATTACHMENT1

	sampler2D gAmbientMap;           // GL_COLOR_ATTACHMENT2
	sampler2D gDiffuseMap;		     // GL_COLOR_ATTACHMENT3
	sampler2D gSpecularMap;		     // GL_COLOR_ATTACHMENT4
	sampler2D gSpecularShininessMap; // GL_COLOR_ATTACHMENT5
};
uniform GBuffer gbuffer;

// Screen
uniform vec2 gScreenSize;