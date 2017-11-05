#include "stdafx.h"

// General
#include "UIButton.h"

UIButton::UIButton() : base()
{
	m_IsTextEnable = false;

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
    m_TextOffset = _image->GetSize() / 2.0f;
	m_TextAlignW = TextAlignW::TEXT_ALIGNW_CENTER;
	m_TextAlignH = TextAlignH::TEXT_ALIGNH_CENTER;

	base::Init(_position, _image->GetSize(), _image);
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

void UIButton::Show()
{
	UIElement::Show();

	buttonStatus = BUTTON_NORMAL;
}

void UIButton::Hide()
{
	UIElement::Hide();

	buttonStatus = BUTTON_DISABLED;
}

On_Mouse_Moved(UIButton)
{
	if (buttonStatus == BUTTON_NORMAL)
	{
		buttonStatus = BUTTON_HOVER;
	}
}

void UIButton::OnMouseLeaved()
{
    if (buttonStatus == BUTTON_HOVER)
    {
        buttonStatus = BUTTON_NORMAL;
    }
}

On_Mouse_Pressed(UIButton)
{
	buttonStatus = BUTTON_PRESSED;

	return true;
}

On_Mouse_Released(UIButton)
{
	if (IsSelected())
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
