#include "stdafx.h"

// General
#include "gbuffer.h"

GBuffer::GBuffer()
{
	gBuffer = 0;
	depthTexture = 0;
	finalTexture = 0;
	ZERO_MEM(textures);
}

GBuffer::~GBuffer()
{
	if (gBuffer != 0)
	{
		glDeleteFramebuffers(1, &gBuffer);
	}

	if (textures[0] != 0)
	{
		glDeleteTextures(ARRAY_SIZE_IN_ELEMENTS(textures), textures);
	}

	if (depthTexture != 0)
	{
		glDeleteTextures(1, &depthTexture);
	}

	if (finalTexture != 0)
	{
		glDeleteTextures(1, &finalTexture);
	}
}

bool GBuffer::Init(unsigned int WindowWidth, unsigned int WindowHeight)
{
	// Create the FBO
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuffer);

	// WorldSpacePos, TexCoord, Normal
	for (uint32_t i = 0; i < 2; i++)
	{
		glGenTextures(1, &textures[i]);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textures[i], 0);
	}

	// Emission, Ambient, Diffuse, Specular, SpecularShininess
	for (uint32_t i = 2; i < 6; i++)
	{
		glGenTextures(1, &textures[i]);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, WindowWidth, WindowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textures[i], 0);
	}

	// depth
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

	// final
	glGenTextures(1, &finalTexture);
	glBindTexture(GL_TEXTURE_2D, finalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT6, GL_TEXTURE_2D, finalTexture, 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		Debug::Error("Gbuffer[]: FB error, status: [0x%x]", status);
		return false;
	}

	// restore default FBO
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	return true;
}

void GBuffer::StartFrame()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuffer);

	// Clear

	glDrawBuffer(GL_COLOR_ATTACHMENT6);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GBuffer::BindForGeomPass()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuffer);

	// Enable write in textures
	GLenum DrawBuffers[] = 
	{
		GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, 
		GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5
	};
	glDrawBuffers(ARRAY_SIZE_IN_ELEMENTS(DrawBuffers), DrawBuffers);
}

void GBuffer::Clear()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuffer);

	// Enable write in textures
	GLenum DrawBuffers[] =
	{
		GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5
	};
	glDrawBuffers(ARRAY_SIZE_IN_ELEMENTS(DrawBuffers), DrawBuffers);

	// ... and clear it
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

	for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(textures); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
	}
}

void GBuffer::BindForFinalPass(GLint _color)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);	// Get data from gBuffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);			// Set target buffer as Default

	glReadBuffer(_color);
	glBlitFramebuffer(0, 0, _Settings->windowSizeX, _Settings->windowSizeY, 
					  0, 0, _Settings->windowSizeX, _Settings->windowSizeY, 
					  GL_COLOR_BUFFER_BIT, GL_LINEAR);
}