#include "stdafx.h"

// General
#include "Font.h"

void Font::Render(cstring _string) const
{
	glBindTexture(GL_TEXTURE_2D, m_TextureOpenglId);

	// Alternative, ignores undrawables (no noticable speed difference)
	glListBase(m_ListOpenglId - 32);
	glCallLists(_string.length(), GL_UNSIGNED_BYTE, _string.c_str());

	glBindTexture(GL_TEXTURE_2D, 0);
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