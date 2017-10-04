#pragma once

class Font : public RefItem
{
public:
	Font(GLuint textureOpenglId, GLuint _fontBufferObj, uint32_t* charWidthArray, uint32_t charHeight);

	 ~Font();

	 void Render(cstring _string, vec2 _offset) const;

	 uint32_t GetStringWidth(cstring _string) const;
	 uint32_t GetHeight() const;

public:
	static const uint32_t SPACE = 32;
	static const uint32_t NUM_CHARS = 192;

private:
	GLuint m_TextureOpenglId;
	GLuint m_FontBuffer;
	uint32_t* m_CharWidthArray;
	uint32_t m_CharHeight;
};
