#include "stdafx.h"

// General
#include "UIElement.h"

// Additional
#include "UIMgr.h"

On_Mouse_Entered(UIElement)
{
    if (m_IsHided)
    {
        return;
    }

    //Modules::log().Info("Mouse Entered [%s]", GetName().c_str());
}

On_Mouse_Moved(UIElement)
{
    if (m_IsHided)
    {
        return;
    }

    // This
    //Modules::log().Info("Mouse moved [%s]", GetName().c_str());

    // Childs
    for (auto it : m_Childs)
    {
        bool beforeMoveSelection = it->IsSelected();
        bool afterMoveSelection = it->CheckSelection(Modules::input().GetMouse());

        if (!beforeMoveSelection && afterMoveSelection)
        {
            it->OnMouseEntered();
        }
        else if (beforeMoveSelection && !afterMoveSelection)
        {
            it->OnMouseLeaved();
        }
        else if (beforeMoveSelection && afterMoveSelection)
        {
            it->OnMouseMoved(_mousePos - it->GetPosition());
        }
        else
        {
            // Mouse outside
        }
    }
}

On_Mouse_Leaved(UIElement)
{
    if (m_IsHided)
    {
        return;
    }

    //Modules::log().Info("Mouse Leaved [%s]", GetName().c_str());
}

On_Mouse_Pressed(UIElement)
{
    if (m_IsHided)
    {
        return false;
    }

    // This
    //Modules::log().Info("Mouse pressed [%s]", GetName().c_str());

    // Childs
    bool result = false;
    for (auto it : m_Childs)
    {
        if (it->IsSelected())
        {
            if (it->OnMouseButtonPressed(_button, _mods, _mousePos - it->GetPosition()))
            {
                result = true;
            }
        }
    }

    return result;
}

On_Mouse_Released(UIElement)
{
    if (m_IsHided)
    {
        return false;
    }

    // This
    //Modules::log().Info("Mouse released [%s]", GetName().c_str());

    // Childs
    bool result = false;
    for (auto it : m_Childs)
    {
        if (it->OnMouseButtonReleased(_button, _mods, _mousePos - it->GetPosition()))
        {
            result = true;
        }
    }
    return result;
}

On_Mouse_WScrolled(UIElement)
{
    if (m_IsHided)
    {
        return false;
    }

    // This
    //Modules::log().Info("Mouse scrolled [%s]", GetName().c_str());

    // Childs
    bool result = false;
    for (auto it : m_Childs)
    {
        if (it->IsSelected())
        {
            if (it->OnMouseWheel(_yoffset))
            {
                result = true;
            }
        }
    }

    return result;
}

On_Keyboard_Pressed(UIElement)
{
    if (m_IsHided)
    {
        return false;
    }

    // This
    //Modules::log().Info("Keyboard pressed [%s]", GetName().c_str());

    // Childs
    bool result = false;
    for (auto it : m_Childs)
    {
        if (it->OnKeyboardPressed(_key, _scancode, _mods))
        {
            result = true;
        }
    }
    return result;
}

On_Keyboard_Released(UIElement)
{
    if (m_IsHided)
    {
        return false;
    }

    // This
    //Modules::log().Info("Keyboard released [%s]", GetName().c_str());

    // Childs
    bool result = false;
    for (auto it : m_Childs)
    {
        if (it->OnKeyboardReleased(_key, _scancode, _mods))
        {
            result = true;
        }
    }
    return result;
}