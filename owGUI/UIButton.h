#pragma once

#include "UIElement.h"

class UIButton : public UIElement {
	typedef UIElement base;
public:
	OW_GUI_DLL_API UIButton();
	virtual ~UIButton();

	OW_GUI_DLL_API void Init(cvec2 _position, Image* _image);

	OW_GUI_DLL_API void SetAction(ArgFunctionBase* _onPress);

	virtual void Enable();
	virtual void Disable();

	V_MOUSE_MOVED;
	V_MOUSE_PRESSED;
	V_MOUSE_RELEASE;
	virtual void OnMouseLeaved();

protected:
	ArgFunctionBase* onPressFunction;

	enum ButtonStatus {
		BUTTON_NORMAL = 0,
		BUTTON_HOVER,
		BUTTON_PRESSED,
		BUTTON_DISABLED
	};
	ButtonStatus buttonStatus;
};

// Set action
/*#define SETBUTTONACTION(_uielement, _funcName)\
{\
	if(_uielement != nullptr){\
		UIButton* elemAsBtn = dynamic_cast<UIButton*>(_uielement);\
		if (elemAsBtn != nullptr) { \
			elemAsBtn->SetAction(MACRO_FUNCTION(_funcName));\
		}\
	}\
}*/

#define SETBUTTONACTION_ARG(_uielement, _className, _instance, _funcName, argType, arg)\
{\
	if(_uielement != nullptr) \
	{\
		UIButton* elemAsBtn = dynamic_cast<UIButton*>(_uielement);\
		if (elemAsBtn != nullptr) \
		{ \
			elemAsBtn->SetAction(CLASS_ARG_FUNCTION(_className, _instance, _funcName, argType, arg));\
		}\
	}\
}
