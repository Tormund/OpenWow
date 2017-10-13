#include "../stdafx.h"

#include "SimpleRenderGBuffer.h"

SimpleRenderGBuffer::SimpleRenderGBuffer() : Technique("shaders/Common_SimpleVertex.vs", "shaders/POST_SimpleRender.fs")
{}

bool SimpleRenderGBuffer::Init()
{
	return true;
}