#include "stdafx.h"

// General
#include "gbuffer.h"

// Additional
#include "Render.h"

bool GBuffer::Init()
{
	rb = _Render->r->createRenderBuffer(Modules::config().windowSizeX, Modules::config().windowSizeY, R_TextureFormats::RGBA16F, true, 4, 4);
	assert1(rb != 0);

	rb2 = _Render->r->createRenderBuffer(Modules::config().windowSizeX, Modules::config().windowSizeY, R_TextureFormats::RGBA16F, true, 4, 4);
	assert1(rb2 != 0);

	return true;
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