#pragma once

#include "UIElement.h"

class UIInput : public UIElement {
	typedef UIElement base;
public:
	OW_GUI_DLL_API UIInput();

	OW_GUI_DLL_API void Init(cvec2 _position, cvec2 _size, InputMode _inputMode = MODE_ANY);

	//

	OW_GUI_DLL_API void SetInputString(cstring _inputString) { text = _inputString; }
	OW_GUI_DLL_API void SetInputMaxLenght(int _inputStringMaxLenght) { inputStringMaxLenght = _inputStringMaxLenght; }
	OW_GUI_DLL_API void SetInputMode(InputMode _inputMode) { inputMode = _inputMode; }

	OW_GUI_DLL_API string Data() const { return text; }

	//

	virtual void Render();

	V_MOUSE_PRESSED;
	V_KEYBD_PRESSED;
	V_CHAR_INPUT;

private:
	int inputStringMaxLenght;

	InputMode inputMode;
};
