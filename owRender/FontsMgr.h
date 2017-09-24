#pragma once

#include "Font.h"

class FontsMgr : public Module, public RefManager2Dim<Font, GLuint>
{
public:
	DEF_MODULE_API(FontsMgr, );

	 Font* Add(cstring _fontFileName, uint32_t _fontSize);
	 Font* Add(File& _fontFileName, uint32_t _fontSize);

	//

	 void Render(cstring _string) const;
	
	 Font* GetMainFont() const { return mainFont; }
	 size_t GetFontsCount() const { return Fonts.size(); }

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
