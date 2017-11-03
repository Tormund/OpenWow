#include "stdafx.h"

// General
#include "InputListenerCollection.h"

vector<InputListenerObject*> InputListenerObjectCollection::m_Objects;

//

bool InputListenerObjectCollection::RegisterObject(InputListenerObject* _Object)
{
    m_Objects.push_back(_Object);

    return true;
}

void InputListenerObjectCollection::UnregisterObject(InputListenerObject* _Object)
{
    m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), _Object), m_Objects.end());
}

//

void InputListenerObjectCollection::OnMouseMoved(cvec2 _mousePos)
{
    for (auto it : m_Objects)
    {
        it->OnMouseMoved(_mousePos);
    }
}

void InputListenerObjectCollection::OnMouseButtonPressed(int _button, int _mods, cvec2 _mousePos)
{
    for (auto it : m_Objects)
    {
        if (it->OnMouseButtonPressed(_button, _mods, _mousePos))
        {
            return;
        }
    }
}

void InputListenerObjectCollection::OnMouseButtonReleased(int _button, int _mods, cvec2 _mousePos)
{
    for (auto it : m_Objects)
    {
        if (it->OnMouseButtonReleased(_button, _mods, _mousePos))
        {
            return;
        }
    }
}

void InputListenerObjectCollection::OnMouseWheel(int _yoffset)
{
    for (auto it : m_Objects)
    {
        if (it->OnMouseWheel(_yoffset))
        {
            return;
        }
    }
}

void InputListenerObjectCollection::OnKeyboardPressed(int _key, int _scancode, int _mods)
{
    for (auto it : m_Objects)
    {
        if (it->OnKeyboardPressed(_key, _scancode, _mods))
        {
            return;
        }
    }
}

void InputListenerObjectCollection::OnKeyboardReleased(int _key, int _scancode, int _mods)
{
    for (auto it : m_Objects)
    {
        if (it->OnKeyboardReleased(_key, _scancode, _mods))
        {
            return;
        }
    }
}

void InputListenerObjectCollection::OnCharInput(uint32 _char)
{
    for (auto it : m_Objects)
    {
        if (it->OnCharInput(_char))
        {
            return;
        }
    }
}
