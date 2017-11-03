#include "stdafx.h"

// General
#include "UIButton.h"

UIButton::UIButton() : base()
{
	textEnable = false;

	onPressFunction = nullptr;
	buttonStatus = BUTTON_NORMAL;
}

UIButton::~UIButton()
{
	if (onPressFunction != nullptr)
	{
		delete onPressFunction;
	}
}

void UIButton::Init(cvec2 _position, Image* _image)
{
	textOffset = vec2(_image->size.x / 2, _image->size.y / 2);
	textAlignW = TextAlignW::TEXT_ALIGNW_CENTER;
	textAlignH = TextAlignH::TEXT_ALIGNH_CENTER;

	base::Init(_position, _image->size, _image);
}

void UIButton::SetAction(Function* _onPress)
{
	if (onPressFunction != nullptr)
	{
		delete onPressFunction;
	}

    assert1(_onPress);
	onPressFunction = _onPress;
}

void UIButton::CallAction(int _button, int _mods, cvec2 _mousePos)
{
    if (onPressFunction != nullptr)
    {
        onPressFunction->operator()();
    }
}

void UIButton::Enable()
{
	UIElement::Enable();

	buttonStatus = BUTTON_NORMAL;
}

void UIButton::Disable()
{
	UIElement::Disable();

	buttonStatus = BUTTON_DISABLED;
}

MOUSE_MOVED_(UIButton)
{
	if (buttonStatus == BUTTON_NORMAL)
	{
		buttonStatus = BUTTON_HOVER;
	}
}

MOUSE_PRESSED(UIButton)
{
	buttonStatus = BUTTON_PRESSED;

	return true;
}

MOUSE_RELEASE(UIButton)
{
	if (IsMouseHover())
	{
		if (buttonStatus == BUTTON_PRESSED)
		{
            CallAction(_button, _mods, _mousePos);

            return true;
		}
		buttonStatus = BUTTON_HOVER;
	}
	else
	{
		buttonStatus = BUTTON_NORMAL;
	}

	return false;
}

void UIButton::OnMouseLeaved()
{
	if (buttonStatus == BUTTON_HOVER)
	{
		buttonStatus = BUTTON_NORMAL;
	}
}
