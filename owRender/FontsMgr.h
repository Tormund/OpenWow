#pragma once

#include "Font.h"

class FontsMgr : public Module, public RefManager1Dim<Font>
{
public:
	DEF_MODULE(FontsMgr);

	Font* Add(cstring _fontFileName, uint32 _fontSize);
	Font* Add(File& _fontFileName, uint32 _fontSize);

	//
	
	Font* GetMainFont() const { return mainFont; }
	size_t GetFontsCount() const { return Fonts.size(); }

protected:
	Font* CreateAction(cstring name) override;
	bool DeleteAction(cstring name) override;

private:
	map<string, Font*> Fonts;

	Font* mainFont;
};

#define _FontsMgr FontsMgr::instance()
