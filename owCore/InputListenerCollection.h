#pragma once

#include "InputListener.h"

class InputListenerObjectCollection
{
public:
    static bool RegisterObject(InputListenerObject* _uiObject);
    static void UnregisterObject(InputListenerObject* _uiObject);

    //

    static void OnMouseMoved(cvec2 _mousePos);
    static void OnMouseButtonPressed(int _button, int _mods, cvec2 _mousePos);
    static void OnMouseButtonReleased(int _button, int _mods, cvec2 _mousePos);
    static void OnMouseWheel(int _yoffset);
    static void OnKeyboardPressed(int _key, int _scancode, int _mods);
    static void OnKeyboardReleased(int _key, int _scancode, int _mods);
    static void OnCharInput(uint32 _char);

private:
    static vector<InputListenerObject*> m_Objects;
};