#include "stdafx.h"

// General
#include "Texture.h"

void Texture::GenerateTexture()
{
	glGenTextures(1, &oglObj);
}

void Texture::Bind(uint8_t _textureUnit) const
{
	glActiveTexture(GL_TEXTURE0 + _textureUnit);
	glBindTexture(GL_TEXTURE_2D, oglObj);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}