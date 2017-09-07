#include "stdafx.h"

// General
#include "FontsMgr.h"

// Additional
#include <freetype/config/ftheader.h>
#include FT_FREETYPE_H

bool FontsMgr::Init()
{
	mainFont = Add("Fonts\\ARIALN.TTF", 14);

	return true;
}

void FontsMgr::Destroy()
{
	for (auto it = Fonts.begin(); it != Fonts.end(); ++it)
	{
		auto font = (*it).second;

		delete font;

		Debug::Info("FontsMgr: Font [%s] destroyed.", (*it).first.c_str());
	}

	Debug::Info("FontsMgr: All fonts destroyed.");
}

Font* FontsMgr::Add(cstring _fontFileName, uint32_t _fontSize)
{
	return RefManager::Add(_fontFileName + "__" + std::to_string(_fontSize));
}

Font* FontsMgr::Add(File& _fontFileName, uint32_t _fontSize)
{
	return RefManager::Add(_fontFileName.Path_Name() + "__" + std::to_string(_fontSize));
}

//

GLuint FontsMgr::GenerateID() // override
{
	GLuint fontListOpenglId;
	glGenTextures(1, &fontListOpenglId);

	return fontListOpenglId;
}

Font* FontsMgr::CreateAction(cstring _nameAndSize, GLuint id) // override
{
	size_t _delimIndex = _nameAndSize.find_last_of("__");
	if (_delimIndex == -1)
	{
		Debug::Error("FontsMgr[%s]: Incorrect font nameAndSize.", _nameAndSize.c_str());
		return nullptr;
	}

	string fontFileName = _nameAndSize.substr(0, _delimIndex - 1);
	uint32_t fontSize = Utils::ToUInt(_nameAndSize.substr(_delimIndex + 1));

	GLuint listOpenglIndex = 0;
	GLuint textureOpenglIndex = 0;
	uint32_t* charWidth = new uint32_t[Font::NUM_CHARS];
	uint32_t charHeight = 0;

	FT_Library ftLibrary;
	FT_Init_FreeType(&ftLibrary);

	File f = fontFileName;
	if (!f.Open())
	{
		Debug::Fatal("FontsMgr[%s]: Error while loading font.", f.Path_Name().c_str());
		return nullptr;
	}

	FT_Face face;
	if (FT_New_Memory_Face(ftLibrary, f.GetData(), f.GetSize(), 0, &face) != 0)
	{
		Debug::Error("FontsMgr: Error while loading font [%s]. Could not load font file.", f.Path_Name().c_str());
		return nullptr;
	}

	if (!(face->face_flags & FT_FACE_FLAG_SCALABLE) || !(face->face_flags & FT_FACE_FLAG_HORIZONTAL))
	{
		Debug::Error("FontsMgr: Error while loading font [%s]. Error setting font size.", f.Path_Name().c_str());
		return nullptr;
	}

	FT_Set_Pixel_Sizes(face, fontSize, 0);

	// Step 2: Find maxAscent/Descent to calculate imageHeight //
	size_t imageHeight = 0;
	size_t imageWidth = 256;
	int maxDescent = 0;
	int maxAscent = 0;
	size_t lineSpace = imageWidth;
	size_t lines = 1;
	size_t charIndex;

	for (uint32_t ch = 0; ch < Font::NUM_CHARS; ++ch)
	{
		// Look up the character in the font file.
		charIndex = FT_Get_Char_Index(face, ch + Font::SPACE);

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

	listOpenglIndex = glGenLists(Font::NUM_CHARS);

	for (uint32_t ch = 0; ch < Font::NUM_CHARS; ++ch)
	{
		size_t charIndex = FT_Get_Char_Index(face, ch + Font::SPACE);

		FT_Load_Glyph(face, charIndex, FT_LOAD_DEFAULT);
		FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

		if (charWidth[ch] > imageWidth - x)
		{
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

		for (uint32_t row = 0; row < face->glyph->bitmap.rows; ++row)
		{
			for (uint32_t pixel = 0; pixel < face->glyph->bitmap.width; ++pixel)
			{
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
	Fonts.insert(make_pair(_nameAndSize, font));

	Debug::Info("FontsMgr: Font [%s] loaded. Size [%d].", f.Path_Name().c_str(), fontSize);

	return font;
}

bool FontsMgr::DeleteAction(cstring name, GLuint id) // override
{
	return true;
}

//

void FontsMgr::Render(cstring _string) const
{
	mainFont->Render(_string);
}

