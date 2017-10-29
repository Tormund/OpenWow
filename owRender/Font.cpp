#include "stdafx.h"

// General
#include "Font.h"

// Additional
#include "TechniquesManager.h"

Font::Font(uint32 _Texture, uint32 _fontGeom, uint32* charWidthArray, uint32 charHeight) :
	RefItem(),
	m_Texture(_Texture),
	m_FontBuffer(_fontGeom),
	m_CharWidthArray(charWidthArray),
	m_CharHeight(charHeight)
{}

Font::~Font()
{
	//glDeleteBuffers(1, &m_FontBuffer);
	//glDeleteTextures(1, &m_TextureOpenglId);

	delete[] m_CharWidthArray;
}

void Font::Render(cstring _string, vec2 _offset) const
{
	_Render->r->setTexture(10, m_Texture, SS_FILTER_BILINEAR | SS_ANISO16 | SS_ADDR_CLAMP, 0);

	_Render->r->setGeometry(m_FontBuffer);
	
	for (uint32 i = 0; i < _string.length(); i++)
	{
		uint8 ch = _string.c_str()[i];
		if (static_cast<uint32>(ch) < SPACE || static_cast<uint32>(ch) - 32 > Font::NUM_CHARS)
		{
			continue;
		}

		_TechniquesMgr->m_UI_Font->SetCharOffset(_offset);
		_offset.x += static_cast<float>(m_CharWidthArray[ch - SPACE]);

		_Render->r->draw(PRIM_TRILIST, (ch - SPACE) * 6, 6);
	}
}

uint32 Font::GetStringWidth(cstring _string) const
{
	uint32 width = 0;

	for (auto it = _string.begin(); it != _string.end(); ++it)
	{
		if (*it < 32)
		{
			continue;
		}

		width += m_CharWidthArray[*it - SPACE];
	}

	return width;
}

uint32 Font::GetHeight() const
{
	return m_CharHeight;
}