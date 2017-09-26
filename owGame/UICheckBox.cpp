#include "stdafx.h"

// General
#include "UICheckBox.h"

UICheckBox::UICheckBox() : base() {
	isChecked = false;
}

UICheckBox::~UICheckBox() {
	for (int i = 0; i < 2; i++)
		delete checkBoxImages[i];
	image = nullptr;
}

void UICheckBox::Init(cvec2 _position) {
	auto checkBoxTexture = _TexturesMgr->Add("images/ui/buttons/checkbox_default.png");
	auto checkBoxSize = vec2(checkBoxTexture->GetSize().x, checkBoxTexture->GetSize().y / 2);

	for (int i = 0; i < 2; i++)
		checkBoxImages[i] = new Image(checkBoxTexture, vec2(0, checkBoxSize.y * i), checkBoxSize);

	base::Init(_position, checkBoxSize, checkBoxImages[0]);
}

void UICheckBox::Render() {
	if (isChecked)
		image = checkBoxImages[1];
	else
		image = checkBoxImages[0];

	base::Render();
}

MOUSE_PRESSED(UICheckBox) {
	isChecked = !isChecked;

	return true;
}
