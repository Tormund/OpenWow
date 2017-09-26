#include "../stdafx.h"

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

void SimpleRenderGBuffer::SetScreenSize(unsigned int Width, unsigned int Height)
{
	setVec2("gScreenSize", (float)Width, (float)Height);
}
