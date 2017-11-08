#include "stdafx.h"

// General
#include "UIWindow.h"

// Additional
#include "UIMgr.h"

UIWindow::UIWindow() : base()
{
	//color = COLOR_RED.Alpha(0.2);
}

void UIWindow::Init(cvec2 _position, cvec2 _size, Image* _image, Color _color)
{
	m_IsTextEnable = false;

	base::Init(_position, _size, _image, _color);
}

void UIWindow::AddContent(UIElement* _element)
{
    m_Content.push_back(_element);
    _element->AttachTo(this);
}
