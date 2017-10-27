#include "stdafx.h"

// General
#include "gbuffer.h"

// Additional
#include "Render.h"

bool GBuffer::Init()
{
	rb = _Render->r->createRenderBuffer(Modules::config().windowSizeX, Modules::config().windowSizeY, R_TextureFormats::RGBA16F, true, 4, 0);
	assert1(rb != 0);

	rb2 = _Render->r->createRenderBuffer(Modules::config().windowSizeX, Modules::config().windowSizeY, R_TextureFormats::RGBA16F, true, 4, 0);
	assert1(rb2 != 0);

	return true;
}

void GBuffer::Clear()
{
	uint32 clearFlags = 0;
	clearFlags |= CLR_DEPTH;
	clearFlags |= CLR_COLOR_RT0;
	clearFlags |= CLR_COLOR_RT1;
	clearFlags |= CLR_COLOR_RT2;
	clearFlags |= CLR_COLOR_RT3;
	_Render->r->clear(clearFlags);
}

void GBuffer::ClearFinalBuffer()
{
	_Render->r->setRenderBuffer(0);
	uint32 clearFlags = 0;
	clearFlags |= CLR_DEPTH;
	clearFlags |= CLR_COLOR_RT0;
	_Render->r->clear(clearFlags);
}

void GBuffer::BindForGeomPass()
{
	_Render->r->setRenderBuffer(rb);
}

void GBuffer::BindForGeomPass2()
{
	_Render->r->setRenderBuffer(rb2);
}

void GBuffer::BindForLightPass()
{
	_Render->r->setRenderBuffer(0);

	for (uint32 i = 0; i < 4; i++)
	{
		_Render->r->setTexture(i, _Render->r->getRenderBufferTex(rb, i), 0, R_TextureUsage::Texture);
	}
}