#include "stdafx.h"

// General
#include "FontsMgr.h"

// Additional
#include <freetype/config/ftheader.h>
#include FT_FREETYPE_H

bool FontsMgr::Init() {
	mainFont = Add("Fonts\\ARIALN.TTF", 14);

	return true;
}

void FontsMgr::Destroy() {
	for (auto it = Fonts.begin(); it != Fonts.end(); ++it) {
		auto font = (*it).second;

		if (glIsList(font->ListOpenglId))
			glDeleteLists(font->ListOpenglId, NUM_CHARS);

		if (glIsTexture(font->TextureOpenglId))
			glDeleteTextures(1, &font->TextureOpenglId);

		delete[] font->CharWidthArray;

		delete font;

		Debug::Info("FontsMgr: Font [%s] destroyed.", (*it).first.c_str());
	}

	Fonts.clear();
	Debug::Info("FontsMgr: All fonts destroyed.");
}

Font* FontsMgr::Add(cstring _fontFileName, uint32_t _fontSize) {
	string fullFontFileName = _fontFileName + "__" + std::to_string(_fontSize);

	// Try get fonst
	auto fontIt = Fonts.find(fullFontFileName);
	if (fontIt != Fonts.end())
	{
		return (*fontIt).second;
	}

	GLuint listOpenglIndex = 0;
	GLuint textureOpenglIndex = 0;
	uint32_t* charWidth = new uint32_t[NUM_CHARS];
	uint32_t charHeight = 0;

	FT_Library ftLibrary;
	FT_Init_FreeType(&ftLibrary);

	File f(_fontFileName);
	if (!f.Open())
	{
		Debug::Fatal("FontsMgr[%s]: Error while loading font.", f.Path_Name().c_str());
		return nullptr;
	}

	FT_Face face;
	if (FT_New_Memory_Face(ftLibrary, f.GetData(), f.GetSize(), 0, &face) != 0)
		Debug::Error("FontsMgr: Error while loading font [%s]. Could not load font file.", f.Path_Name().c_str());

	if (!(face->face_flags & FT_FACE_FLAG_SCALABLE) || !(face->face_flags & FT_FACE_FLAG_HORIZONTAL))
		Debug::Error("FontsMgr: Error while loading font [%s]. Error setting font size.", f.Path_Name().c_str());

	FT_Set_Pixel_Sizes(face, _fontSize, 0);

	// Step 2: Find maxAscent/Descent to calculate imageHeight //
	size_t imageHeight = 0;
	size_t imageWidth = 256;
	int maxDescent = 0;
	int maxAscent = 0;
	size_t lineSpace = imageWidth;
	size_t lines = 1;
	size_t charIndex;

	for (uint32_t ch = 0; ch < NUM_CHARS; ++ch) {
		// Look up the character in the font file.
		charIndex = FT_Get_Char_Index(face, ch + SPACE);

		// RenderPhase the current glyph.
		FT_Load_Glyph(face, charIndex, FT_LOAD_RENDER);

		charWidth[ch] = (face->glyph->metrics.horiAdvance >> 6);
		// If the line is full go to the next line
		if (charWidth[ch] > lineSpace)
		{
			lineSpace = imageWidth;
			++lines;
		}
		lineSpace -= charWidth[ch];

		maxAscent = max(face->glyph->bitmap_top, maxAscent);
		maxDescent = max((int)face->glyph->bitmap.rows - (int)face->glyph->bitmap_top, maxDescent);
	}

	charHeight = maxAscent + maxDescent;

	size_t neededHeight = (maxAscent + maxDescent) * lines;

	// Get the first power of two in which it will fit
	imageHeight = 16;
	while (imageHeight < neededHeight)
		imageHeight <<= 1;

	// Step 3: Generation of the actual texture //

	unsigned char* image = new unsigned char[imageHeight * imageWidth];
	memset(image, 0, imageHeight * imageWidth);

	// These are the cameraPosition at which to draw the next glyph
	size_t x = 0;
	size_t y = maxAscent;

	listOpenglIndex = glGenLists(NUM_CHARS);

	for (uint32_t ch = 0; ch < NUM_CHARS; ++ch) {
		size_t charIndex = FT_Get_Char_Index(face, ch + SPACE);

		FT_Load_Glyph(face, charIndex, FT_LOAD_DEFAULT);
		FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

		if (charWidth[ch] > imageWidth - x) {
			x = 0;
			y += (maxAscent + maxDescent);
		}

		double texX1 = (double)(x) / imageWidth;
		double texX2 = (double)(x + charWidth[ch]) / imageWidth;
		double texY1 = (double)(y - maxAscent) / imageHeight;
		double texY2 = texY1 + (double)(charHeight) / imageHeight;

		glNewList(listOpenglIndex + ch, GL_COMPILE);
		{
			glBegin(GL_QUADS);
			{
				glTexCoord2d(texX1, texY1);  glVertex2i(0, 0);
				glTexCoord2d(texX2, texY1);  glVertex2i(charWidth[ch], 0);
				glTexCoord2d(texX2, texY2);  glVertex2i(charWidth[ch], charHeight);
				glTexCoord2d(texX1, texY2);  glVertex2i(0, charHeight);
			}
			glEnd();
			glTranslated(charWidth[ch], 0, 0);
		}
		glEndList();

		for (uint32_t row = 0; row < face->glyph->bitmap.rows; ++row) {
			for (uint32_t pixel = 0; pixel < face->glyph->bitmap.width; ++pixel) {
				image[(x + face->glyph->bitmap_left + pixel) + (y - face->glyph->bitmap_top + row) * imageWidth] = face->glyph->bitmap.buffer[pixel + row * face->glyph->bitmap.pitch];
			}
		}

		x += charWidth[ch];
	}

	glGenTextures(1, &textureOpenglIndex);
	glBindTexture(GL_TEXTURE_2D, textureOpenglIndex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA8, imageWidth, imageHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, image);
	glBindTexture(GL_TEXTURE_2D, 0);

	delete[] image;

	FT_Done_Face(face);
	FT_Done_FreeType(ftLibrary);

	//

	Font* font = new Font(textureOpenglIndex, listOpenglIndex, charWidth, charHeight);
	Fonts.insert(make_pair(fullFontFileName, font));

	Debug::Info("FontsMgr: Font [%s] loaded. Size [%d].", f.Path_Name().c_str(), _fontSize);

	return font;
}

bool FontsMgr::Delete(cstring _fontFileName) {
	for (auto it = Fonts.begin(); it != Fonts.end(); ++it)
		if ((*it).first == _fontFileName)
			return Delete((*it).second);

	return false;
}

bool FontsMgr::Delete(Font* _font) {
	for (auto it = Fonts.begin(); it != Fonts.end(); ++it)
		if ((*it).second == _font) {
			if (glIsList(_font->ListOpenglId))
				glDeleteLists(_font->ListOpenglId, NUM_CHARS);

			if (glIsTexture(_font->TextureOpenglId))
				glDeleteTextures(1, &_font->TextureOpenglId);

			delete[] _font->CharWidthArray;

			delete _font;

			Debug::Info("FontsMgr: Font [%s] destroyed.", (*it).first.c_str());

			// Delete from map<>
			Fonts.erase(it);

			return true;
		}

	return false;
}

void FontsMgr::Render(cstring _string) const {
	Render(mainFont, _string);
}

void FontsMgr::Render(Font* _font, cstring _string) const {
	glBindTexture(GL_TEXTURE_2D, _font->TextureOpenglId);

	// Alternative, ignores undrawables (no noticable speed difference)
	glListBase(_font->ListOpenglId - 32);
	glCallLists(_string.length(), GL_UNSIGNED_BYTE, _string.c_str());

	glBindTexture(GL_TEXTURE_2D, 0);
}

uint32_t FontsMgr::GetFontWidth(Font* _font, cstring _string) const {
	uint32_t width = 0;

	for (auto it = _string.begin(); it != _string.end(); ++it) {
		width += _font->CharWidthArray[*it - SPACE];
	}

	return width;
}

uint32_t FontsMgr::GetFontHeight(Font* _font) const {
	return _font->CharHeight;
}
