#pragma once

struct Font {
	Font(GLuint _TextureOpenglId, GLuint _ListOpenglId, uint32_t* _CharWidthArray, uint32_t _CharHeight) {
		TextureOpenglId = _TextureOpenglId;
		ListOpenglId = _ListOpenglId;
		CharWidthArray = _CharWidthArray;
		CharHeight = _CharHeight;
	}

	GLuint TextureOpenglId;
	GLuint ListOpenglId;
	uint32_t* CharWidthArray;
	uint32_t CharHeight;
};
