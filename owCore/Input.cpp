#include "stdafx.h"

// General
#include "Input.h"

// Additional
#include "InputListenerCollection.h"

Input::Input()
{
    for (int i = 0; i < OW_KEYSCOUNT; i++)
    {
        keyState[i] = false;
    }

    for (int i = 0; i < OW_MOUSEBUTTONSCOUNT; i++)
    {
        mouseButtonState[i] = false;
    }
}

Input::~Input()
{}

//

void Input::MousePositionCallback(cvec2 _mousePos)
{
    mousePos = _mousePos;
    InputListenerObjectCollection::OnMouseMoved(mousePos);
}

void Input::MouseCallback(int button, int action, int mods)
{
    if (action == OW_PRESS)
    {
        mouseButtonState[button] = true;
        InputListenerObjectCollection::OnMouseButtonPressed(button, action, mousePos);
    }
    else if (action == OW_RELEASE)
    {
        mouseButtonState[button] = false;
        InputListenerObjectCollection::OnMouseButtonReleased(button, action, mousePos);
    }
}

void Input::MouseScrollCallback(int yoffset)
{
    InputListenerObjectCollection::OnMouseWheel(yoffset);
}

void Input::KeyboardCallback(int key, int scancode, int action, int mods)
{
    if (action == OW_PRESS || action == OW_REPEAT)
    {
        keyState[key] = true;
        InputListenerObjectCollection::OnKeyboardPressed(key, scancode, mods);
    }
    else if (action == OW_RELEASE)
    {
        keyState[key] = false;
        InputListenerObjectCollection::OnKeyboardReleased(key, scancode, mods);
    }
}

void Input::CharCallback(uint32 _char)
{
    InputListenerObjectCollection::OnCharInput(_char);
}
