#pragma once

#include "UIElement.h"

class UIInput : public UIElement
{
    typedef UIElement base;
public:
    UIInput();

    void Init(cvec2 _position, cvec2 _size, InputMode _inputMode = MODE_ANY);

    //

    void SetInputString(cstring _inputString) { m_Text = _inputString; }
    void SetInputMaxLenght(int _inputStringMaxLenght) { inputStringMaxLenght = _inputStringMaxLenght; }
    void SetInputMode(InputMode _inputMode) { inputMode = _inputMode; }

    string Data() const { return m_Text; }

    //

    virtual void OnRenderUI();

    V_MOUSE_PRESSED;
    V_KEYBD_PRESSED;
    V_CHAR_INPUT;

private:
    int inputStringMaxLenght;

    InputMode inputMode;
};
