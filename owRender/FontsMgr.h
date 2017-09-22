#pragma once

#include "Font.h"

class FontsMgr : public Module, public RefManager2Dim<Font, GLuint>
{
public:
	DEF_MODULE_API(FontsMgr, OW_RENDER_DLL_API);

	OW_RENDER_DLL_API Font* Add(cstring _fontFileName, uint32_t _fontSize);
	OW_RENDER_DLL_API Font* Add(File& _fontFileName, uint32_t _fontSize);

	//

	OW_RENDER_DLL_API void Render(cstring _string) const;
	
	OW_RENDER_DLL_API Font* GetMainFont() const { return mainFont; }
	OW_RENDER_DLL_API size_t GetFontsCount() const { return Fonts.size(); }

protected:
	GLuint GenerateID() override;
	Font* CreateAction(cstring name, GLuint id) override;
	bool DeleteAction(cstring name, GLuint id) override;

private:
	map<string, Font*> Fonts;

	Font* mainFont;

	//


};

#define _FontsMgr FontsMgr::instance()
