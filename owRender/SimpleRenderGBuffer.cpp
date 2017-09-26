#include "stdafx.h"

#include "SimpleRenderGBuffer.h"

SimpleRenderGBuffer::SimpleRenderGBuffer() : Technique("shaders/simpleRender.glsl")
{}

bool SimpleRenderGBuffer::Init()
{
	if (!CompileProgram("SimpleRender"))
	{
		return false;
	}
}

void SimpleRenderGBuffer::SetWVP(cmat4 WVP)
{
	setMat4("gWVP", WVP);
}


void SimpleRenderGBuffer::SetPositionTextureUnit(unsigned int TextureUnit)
{
	setInt("gbuffer.gPositionMap", TextureUnit);
}
void SimpleRenderGBuffer::SetColorTextureUnit(unsigned int TextureUnit)
{
	setInt("gbuffer.gColorMap", TextureUnit);
}
void SimpleRenderGBuffer::SetNormalTextureUnit(unsigned int TextureUnit)
{
	setInt("gbuffer.gNormalMap", TextureUnit);
}
void SimpleRenderGBuffer::SetSpecularTextureUnit(unsigned int TextureUnit)
{
	setInt("gbuffer.gSpecularMap", TextureUnit);
}




void SimpleRenderGBuffer::SetScreenSize(unsigned int Width, unsigned int Height)
{
	setVec2("gScreenSize", (float)Width, (float)Height);
}
