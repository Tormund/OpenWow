#pragma once

#include "UIElement.h"

class UICheckBox : public UIElement {
	typedef UIElement base;
public:
	UICheckBox();
	virtual ~UICheckBox();

	 void Init(cvec2 _position);

	bool IsChecked() const { return isChecked; }

	virtual void OnRenderUI();

	V_MOUSE_PRESSED;

protected:
	bool isChecked;
	Image* checkBoxImages[2];
};
