#pragma once

#include "UIElement.h"

class UIWindow : public UIElement {
	friend UIElement;
	friend UIMgr;
	typedef UIElement base;

public:
	OW_GUI_DLL_API UIWindow();

	OW_GUI_DLL_API void Init(cvec2 _position, cvec2 _size, Image* _image, Color _color = COLOR_EMPTY);

	// Parent & childs functional

	inline vector<UIElement*>& GetChilds() { return childs; }
	inline size_t GetChildsCount() const { return childs.size(); }

	// Common functional

	virtual void Render();

	V_MOUSE_MOVED;
	virtual void OnMouseLeaved();
	V_MOUSE_PRESSED;
	V_MOUSE_RELEASE;
	V_MOUSE_WHEEL;
	V_KEYBD_PRESSED;
	V_KEYBD_RELEASE;

private:
	void DeleteChilds();

private:
	vector<UIElement*> childs;
};
