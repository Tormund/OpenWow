#pragma once

class Font : public RefItem
{
public:
	OW_RENDER_DLL_API Font(GLuint textureOpenglId, GLuint listOpenglId, uint32_t* charWidthArray, uint32_t charHeight) :
		RefItem(),
		m_TextureOpenglId(textureOpenglId),
		m_ListOpenglId(listOpenglId),
		m_CharWidthArray(charWidthArray),
		m_CharHeight(charHeight)
	{}

	~Font()
	{
		if (glIsList(m_ListOpenglId))
		{
			glDeleteLists(m_ListOpenglId, NUM_CHARS);
		}

		if (glIsTexture(m_TextureOpenglId))
		{
			glDeleteTextures(1, &m_TextureOpenglId);
		}

		delete[] m_CharWidthArray;
	}

	OW_RENDER_DLL_API void Render(cstring _string) const;

	OW_RENDER_DLL_API uint32_t GetStringWidth(cstring _string) const;
	OW_RENDER_DLL_API uint32_t GetHeight() const;

public:
	static const uint32_t SPACE = 32;
	static const uint32_t NUM_CHARS = 96;

private:
	GLuint m_TextureOpenglId;
	GLuint m_ListOpenglId;
	uint32_t* m_CharWidthArray;
	uint32_t m_CharHeight;
};
