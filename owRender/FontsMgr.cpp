#include "stdafx.h"

// General
#include "FontsMgr.h"

// Additional
#include <freetype/config/ftheader.h>
#include FT_FREETYPE_H

//

struct Font_Vertex
{
	vec2 vertex;
	vec2 textureCoord;
};

//

bool FontsMgr::Init()
{
	mainFont = Add("Fonts\\Consolas.TTF", 12);

	return true;
}

void FontsMgr::Destroy()
{
	DeleteAll();

	Modules::log().Info("FontsMgr[]: All fonts destroyed.");
}

Font* FontsMgr::Add(cstring _fontFileName, uint32 _fontSize)
{
	return RefManager1Dim::Add(_fontFileName + "__" + std::to_string(_fontSize));
}

Font* FontsMgr::Add(File& _fontFileName, uint32 _fontSize)
{
	return RefManager1Dim::Add(_fontFileName.Path_Name() + "__" + std::to_string(_fontSize));
}

//

Font* FontsMgr::CreateAction(cstring _nameAndSize)
{
	size_t _delimIndex = _nameAndSize.find_last_of("__");
	if (_delimIndex == -1)
	{
		Modules::log().Error("FontsMgr[%s]: Incorrect font nameAndSize.", _nameAndSize.c_str());
		return nullptr;
	}

	string fontFileName = _nameAndSize.substr(0, _delimIndex - 1);
	uint32 fontSize = Utils::ToUInt(_nameAndSize.substr(_delimIndex + 1));

	File f = fontFileName;
	if (!f.Open())
	{
		Modules::log().Fatal("FontsMgr[%s]: Error while loading font.", f.Path_Name().c_str());
		return nullptr;
	}

	GLuint textureOpenglIndex = 0;
	uint32* charWidth = new uint32[Font::NUM_CHARS];
	uint32 charHeight = 0;

	FT_Library ftLibrary;
	FT_Init_FreeType(&ftLibrary);

	FT_Face face;
	if (FT_New_Memory_Face(ftLibrary, f.GetData(), f.GetSize(), 0, &face) != 0)
	{
		Modules::log().Error("FontsMgr[%s]: Error while loading font. Could not load font file.", f.Path_Name().c_str());

		// Unload
		FT_Done_Face(face);
		FT_Done_FreeType(ftLibrary);
		return nullptr;
	}

	if (!(face->face_flags & FT_FACE_FLAG_SCALABLE) || !(face->face_flags & FT_FACE_FLAG_HORIZONTAL))
	{
		Modules::log().Error("FontsMgr[%s]: Error while loading font. Error setting font size.", f.Path_Name().c_str());
		
		// Unload
		FT_Done_Face(face);
		FT_Done_FreeType(ftLibrary);
		return nullptr;
	}

	FT_Set_Pixel_Sizes(face, fontSize, 0);

	// Step 2: Find maxAscent/Descent to calculate imageHeight //
	size_t imageHeight = 0;
	size_t imageWidth = 512;
	int maxDescent = 0;
	int maxAscent = 0;
	size_t lineSpace = imageWidth;
	size_t lines = 1;
	size_t charIndex;

	for (uint32 ch = 0; ch < Font::NUM_CHARS; ++ch)
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
	{
		imageHeight <<= 1;
	}

	// Step 3: Generation of the actual texture //

	uint8* image = new uint8[imageHeight * imageWidth];
	memset(image, 0x00, imageHeight * imageWidth);

	// These are the cameraPosition at which to draw the next glyph
	size_t x = 0;
	size_t y = maxAscent;

	vector<Font_Vertex> fontVertices;
	float xOffset = 0.0f;

	for (uint32 ch = 0; ch < Font::NUM_CHARS; ++ch)
	{
		//Modules::log().Warn("Char [%c] %d", char(ch), ch);
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

		fontVertices.push_back({vec2(0.0f,          0.0f),       vec2(texX1, texY1)});
		fontVertices.push_back({vec2(charWidth[ch], 0.0f),       vec2(texX2, texY1)});
		fontVertices.push_back({vec2(0.0f,          charHeight), vec2(texX1, texY2)});

		fontVertices.push_back({vec2(0.0f,          charHeight), vec2(texX1, texY2)});
		fontVertices.push_back({vec2(charWidth[ch], 0.0f),       vec2(texX2, texY1)});
		fontVertices.push_back({vec2(charWidth[ch], charHeight), vec2(texX2, texY2)});
		
	
		for (uint32 row = 0; row < face->glyph->bitmap.rows; ++row)
		{
			for (uint32 pixel = 0; pixel < face->glyph->bitmap.width; ++pixel)
			{
				image[(x + face->glyph->bitmap_left + pixel) + (y - face->glyph->bitmap_top + row) * imageWidth] = face->glyph->bitmap.buffer[pixel + row * face->glyph->bitmap.pitch];
			}
		}

		x += charWidth[ch];
	}

	// Font vertex buffer
	GLuint globalBuffer;
	glGenBuffers(1, &globalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, globalBuffer);
	glBufferData(GL_ARRAY_BUFFER, Font::NUM_CHARS * 6 * sizeof(Font_Vertex), fontVertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Font texture
	glGenTextures(1, &textureOpenglIndex);
	glBindTexture(GL_TEXTURE_2D, textureOpenglIndex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, imageWidth, imageHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, image);
	glBindTexture(GL_TEXTURE_2D, 0);

	delete[] image;

	FT_Done_Face(face);
	FT_Done_FreeType(ftLibrary);

	//

	Font* font = new Font(textureOpenglIndex, globalBuffer, charWidth, charHeight);
	Fonts.insert(make_pair(_nameAndSize, font));

	Modules::log().Info("FontsMgr[%s]: Font loaded. Size [%d].", f.Path_Name().c_str(), fontSize);

	return font;
}

bool FontsMgr::DeleteAction(cstring name)
{
	return true;
}

