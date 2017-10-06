#include "../stdafx.h"

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
	textEnable = false;

	base::Init(_position, _size, _image, _color);
}

void UIWindow::Render()
{
	// Render me
	base::Render();

	// Render childs
	for (auto it = childs.begin(); it != childs.end(); ++it)
	{
		(*it)->Render();
	}
}

MOUSE_MOVED_(UIWindow)
{
	for (auto it = childs.begin(); it != childs.end(); ++it)
	{
		if ((*it)->CheckMouseHover())
		{
			if (!(*it)->IsDisabled())
				(*it)->OnMouseMoved(_mousePos - (*it)->GetPosition());
		}
		else
			(*it)->OnMouseLeaved();
	}
}

MOUSE_PRESSED(UIWindow)
{
	bool result = false;
	for (auto it = childs.begin(); it != childs.end(); ++it)
		if (!(*it)->IsDisabled())
			if ((*it)->IsMouseHover())
				if ((*it)->OnMouseButtonPressed(_button, _mods, _mousePos - (*it)->GetPosition()))
					result = true;

	return result;
}

MOUSE_RELEASE(UIWindow)
{
	bool result = false;
	for (auto it = childs.begin(); it != childs.end(); ++it)
		if (!(*it)->IsDisabled())
			if ((*it)->OnMouseButtonReleased(_button, _mods, _mousePos - (*it)->GetPosition()))
				result = true;

	return result;
}

MOUSE_WHEEL(UIWindow)
{
	bool result = false;
	for (auto it = childs.begin(); it != childs.end(); ++it)
		if (!(*it)->IsDisabled())
			if ((*it)->IsMouseHover())
				if ((*it)->OnMouseWheel(_yoffset))
					result = true;

	return result;
}

KEYBD_PRESSED(UIWindow)
{
	bool result = false;
	for (auto it = childs.begin(); it != childs.end(); ++it)
		if (!(*it)->IsDisabled())
			if ((*it)->OnKeyboardPressed(_key, _scancode, _mods))
				result = true;

	return result;
}

KEYBD_RELEASE(UIWindow)
{
	bool result = false;
	for (auto it = childs.begin(); it != childs.end(); ++it)
		if (!(*it)->IsDisabled())
			if ((*it)->OnKeyboardReleased(_key, _scancode, _mods))
				result = true;

	return result;
}

void UIWindow::OnMouseLeaved()
{
	for (auto it = childs.begin(); it != childs.end(); ++it)
		(*it)->OnMouseLeaved();
}

//

void UIWindow::DeleteChilds()
{
	for (auto chIt = childs.begin(); chIt != childs.end(); )
	{
		// If child elem is UIWindow then delete childs
		if (auto childAsUIWindow = dynamic_cast<UIWindow*>(*chIt))
			childAsUIWindow->DeleteChilds();

		_UIMgr->DeleteUIElement(*chIt);

		chIt = childs.erase(chIt);
	}
}
