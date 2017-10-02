#include "stdafx.h"

// General
#include "Font.h"

// Additional
#include "TechniquesManager.h"

Font::Font(GLuint textureOpenglId, GLuint _fontBufferObj, uint32_t* charWidthArray, uint32_t charHeight) :
	RefItem(),
	m_TextureOpenglId(textureOpenglId),
	m_FontBuffer(_fontBufferObj),
	m_CharWidthArray(charWidthArray),
	m_CharHeight(charHeight)
{}

Font::~Font()
{
	glDeleteBuffers(1, &m_FontBuffer);
	glDeleteTextures(1, &m_TextureOpenglId);

	delete[] m_CharWidthArray;
}

void Font::Render(cstring _string, vec2 _offset) const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_FontBuffer);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const GLvoid*)(0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const GLvoid*)(sizeof(vec2)));

	glBindTexture(GL_TEXTURE_2D, m_TextureOpenglId);

	for (uint32_t i = 0; i < _string.length(); i++)
	{
		uint8_t ch = _string.c_str()[i];

		_TechniquesMgr->m_UI_Font->SetCharOffset(_offset);
		_offset.x += static_cast<float>(m_CharWidthArray[ch - SPACE]);

		glDrawArrays(GL_QUADS, (ch - SPACE) * 4, 4);
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

uint32_t Font::GetStringWidth(cstring _string) const
{
	uint32_t width = 0;

	for (auto it = _string.begin(); it != _string.end(); ++it)
	{
		width += m_CharWidthArray[*it - SPACE];
	}

	return width;
}

uint32_t Font::GetHeight() const
{
	return m_CharHeight;
}