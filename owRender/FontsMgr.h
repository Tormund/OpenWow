#pragma once

class Module;

struct Font;

class FontsMgr : public Module {
public:
	DEF_MODULE(FontsMgr, OW_RENDER_DLL_API);

	OW_RENDER_DLL_API Font* Add(cstring _fontFileName, uint32_t _fontSize);
	OW_RENDER_DLL_API bool Delete(cstring _fontFileName);
	OW_RENDER_DLL_API bool Delete(Font* _font);

	OW_RENDER_DLL_API void Render(cstring _string) const;
	OW_RENDER_DLL_API void Render(Font* _font, cstring _string) const;

	OW_RENDER_DLL_API uint32_t GetFontWidth(Font* _font, cstring _string) const;
	OW_RENDER_DLL_API uint32_t GetFontHeight(Font* _font) const;

	OW_RENDER_DLL_API Font* GetMainFont() const { return mainFont; }
	OW_RENDER_DLL_API int GetFontsCount() const { return Fonts.size(); }

private:
	map<string, Font*> Fonts;

	Font* mainFont;

	//

	static const uint32_t SPACE = 32;
	static const uint32_t NUM_CHARS = 96;
};

#define _FontsMgr FontsMgr::instance()
