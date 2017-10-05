#pragma once

class Font : public RefItem
{
public:
	Font(GLuint textureOpenglId, GLuint _fontBufferObj, uint32* charWidthArray, uint32 charHeight);

	 ~Font();

	 void Render(cstring _string, vec2 _offset) const;

	 uint32 GetStringWidth(cstring _string) const;
	 uint32 GetHeight() const;

public:
	static const uint32 SPACE = 32;
	static const uint32 NUM_CHARS = 192;

private:
	GLuint m_TextureOpenglId;
	GLuint m_FontBuffer;
	uint32* m_CharWidthArray;
	uint32 m_CharHeight;
};
