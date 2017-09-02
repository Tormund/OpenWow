#pragma once

#include "UIElement.h"

class UICheckBox : public UIElement {
	typedef UIElement base;
public:
	UICheckBox();
	virtual ~UICheckBox();

	OW_GAME_DLL_API void Init(cvec2 _position);

	bool IsChecked() const { return isChecked; }

	virtual void Render();
	V_MOUSE_PRESSED;

protected:
	bool isChecked;
	Image* checkBoxImages[2];
};
