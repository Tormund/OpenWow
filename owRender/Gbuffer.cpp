#include "stdafx.h"

// General
#include "gbuffer.h"

GBuffer::GBuffer()
{
	gBuffer = 0;
	depthTexture = 0;
	//finalTexture = 0;
	OW_ZERO_MEM(textures);
}

GBuffer::~GBuffer()
{
	if (gBuffer != 0)
	{
		glDeleteFramebuffers(1, &gBuffer);
	}

	if (textures[0] != 0)
	{
		glDeleteTextures(OW_COUNT_ELEMENTS(textures), textures);
	}

	if (depthTexture != 0)
	{
		glDeleteTextures(1, &depthTexture);
	}

	//if (finalTexture != 0)
	//{
	//	glDeleteTextures(1, &finalTexture);
	//}
}

bool GBuffer::Init()
{
	//finalTexture = _finalTexture;
	//assert1(glIsTexture(finalTexture));

	// Create the FBO
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuffer);

	// WorldSpacePos, Normal
	for (uint32 i = 0; i < 2; i++)
	{
		glGenTextures(1, &textures[i]);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, Modules::config().windowSizeX, Modules::config().windowSizeY, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textures[i], 0);
	}

	// Ambient, Diffuse, Specular, SpecularShininess
	for (uint32 i = 2; i < 6; i++)
	{
		glGenTextures(1, &textures[i]);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Modules::config().windowSizeX, Modules::config().windowSizeY, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textures[i], 0);
	}

	// depth
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, Modules::config().windowSizeX, Modules::config().windowSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

	// final
	//glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT6, GL_TEXTURE_2D, finalTexture, 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		Modules::log().Error("Gbuffer[]: FB error, status: [0x%x]", status);
		return false;
	}

	// restore default FBO
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	return true;
}

void GBuffer::StartFrame(GLuint _finalTexture)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuffer);

	// final
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT6, GL_TEXTURE_2D, _finalTexture, 0);

	// Clear final texure
	glDrawBuffer(GL_COLOR_ATTACHMENT6);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void GBuffer::Clear()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuffer);

	// Enable write in textures
	GLenum DrawBuffers[] =
	{
		GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,                                              // vec3
		GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5   // vec4
	};
	glDrawBuffers(OW_COUNT_ELEMENTS(DrawBuffers), DrawBuffers);

	// ... and clear it
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void GBuffer::ClearFinalBuffer()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuffer);


}

void GBuffer::BindForGeomPass()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuffer);

	// Enable write in textures
	GLenum DrawBuffers[] =
	{
		GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,                                              // vec3
		GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5   // vec4
	};
	glDrawBuffers(OW_COUNT_ELEMENTS(DrawBuffers), DrawBuffers);
}

void GBuffer::BindForStencilPass()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuffer);

	// Disable write in textures

	glDrawBuffer(GL_NONE);
}

void GBuffer::BindForLightPass()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuffer);

	// Enable process result shaders

	glDrawBuffer(GL_COLOR_ATTACHMENT6);

	for (unsigned int i = 0; i < OW_COUNT_ELEMENTS(textures); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
	}
}

void GBuffer::BindForFinalPass(GLint _color)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glReadBuffer(_color);

	glBlitFramebuffer(
		0, 0, Modules::config().windowSizeX, Modules::config().windowSizeY,
		0, 0, Modules::config().windowSizeX, Modules::config().windowSizeY,
		GL_COLOR_BUFFER_BIT, GL_LINEAR);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}