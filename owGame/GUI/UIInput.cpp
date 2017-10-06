#include "../stdafx.h"

// General
#include "UIInput.h"

// Additional
#include "UIMgr.h"

UIInput::UIInput() : base()
{
	inputStringMaxLenght = 15;
	inputMode = InputMode::MODE_ANY;

	textEnable = true;
	text = "";
}

void UIInput::Init(cvec2 _position, cvec2 _size, InputMode _inputMode)
{
	textOffset = vec2(6, _size.y / 2);
	textAlignW = TextAlignW::TEXT_ALIGNW_LEFT;
	textAlignH = TextAlignH::TEXT_ALIGNH_CENTER;

	inputMode = _inputMode;

	auto inputTexture = _TexturesMgr->Add("images/ui/buttons/listbox_char.png");

	base::Init(_position, _size, new Image(inputTexture));
}

void UIInput::Render()
{
	base::Render();

	if (_UIMgr->GetFocus() == this)
		_Render->RenderRectangle(GetPosition() + vec2(2, 2), size - vec2(4, 4), false, COLOR_WHITE);
}

MOUSE_PRESSED(UIInput)
{
	if (_UIMgr->GetFocus() == this)
		_UIMgr->SetFocus(nullptr);
	else
		_UIMgr->SetFocus(this);

	return true;
}

KEYBD_PRESSED(UIInput)
{
	if (_UIMgr->GetFocus() != this)
		return false;

	if (_key == OW_KEY_BACKSPACE && text.length() > 0)
	{
		text.pop_back();

		return true;
	}

	return false;
}

CHAR_INPUT(UIInput)
{
	if (_UIMgr->GetFocus() != this)
		return false;

	if (text.length() > inputStringMaxLenght - 1)
		return true;

	if (inputMode == MODE_CHARACTERS && (_char < 'a' || _char > 'z') && (_char < 'A' || _char > 'Z'))
		return true;

	if (inputMode == MODE_NUMBERS && (_char < '0' || _char > '9'))
		return true;

	text += char(_char);

	return true;
}
