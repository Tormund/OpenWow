#include "stdafx.h"

// Common
#include "UIElement.h"

// General
#include "UIMgr.h"

bool UIMgr::Init()
{
    m_IDCounter = 0;
    m_RootElement = nullptr;
    m_FocusedElement = nullptr;

    //

    UpdatableObject::Register();
    RenderableUIObject::Register(500);
    InputListenerObject::Register();
    return true;
}

void UIMgr::Destroy()
{
    assert1(m_RootElement != nullptr);
    m_RootElement->Delete();

    //

    UpdatableObject::Unregister();
    RenderableUIObject::Unregister();
    InputListenerObject::Unregister();
}

//

void UIMgr::AttachToRoot(UIElement* _element)
{
    assert1(m_RootElement != nullptr);
    AttachElementToParent(_element, m_RootElement);
}

void UIMgr::DetachFromRoot(UIElement * _element, bool _checkChilds)
{

}

void UIMgr::AttachElementToParent(UIElement* _element, UIElement* _parent)
{
    // If element already has a parent
    if (_element->m_Parent != nullptr)
    {
        Modules::log().Error("UIElement: Can't attach element [%s], because element already have a parent [%s]!", _element->GetName().c_str(), _element->m_Parent->GetName().c_str());
        return;
    }

    // Attach to root case
    if (_parent == nullptr)
    {
        _UIMgr->AttachToRoot(_element);
        return;
    }

    // Add element in parent childs
    _parent->m_Childs.push_back(_element);

    // Set element parent
    _element->m_Parent = _parent;
    _element->m_Deep = _parent->m_Deep + 1;
    _element->m_Deep += _element->m_DeepAdding;
}

//

void UIMgr::SetFocus(UIElement* _element) 
{
    assert1(_element != nullptr);

    if (_element->m_Deep > m_FocusedElement->m_Deep)
    {
        m_FocusedElement = _element;
    }
}

//

void UIMgr::Update(double t, double dt)
{
    // Detach from parent
    for (auto it = m_ObjectsToDetach.begin(); it != m_ObjectsToDetach.end(); )
    {
        DetachFromParent(*it);

        it = m_ObjectsToDetach.erase(it);
    }

    // Delete
    for (auto it = m_ObjectsToDelete.begin(); it != m_ObjectsToDelete.end(); )
    {
        DetachFromParent(*it);

        (*it)->DeleteChilds();

        Modules::log().Info("UI: Element [%s] deleted", (*it)->GetName().c_str());
        DeleteUIElement(*it);

        it = m_ObjectsToDelete.erase(it);
    }

    // Update window
    assert1(m_RootElement != nullptr);
    m_FocusedElement = m_RootElement;
    m_RootElement->Update();
}

void UIMgr::RenderUI()
{
    // Update window
    assert1(m_RootElement != nullptr);
    m_RootElement->RenderUI();

    // Render debug
    if (m_FocusedElement != nullptr)
    {
        _Render->RenderRectangleOutline(m_FocusedElement->GetPosition(), m_FocusedElement->GetSize(), COLOR_BLUE);
    }
}

//

string UIMgr::GetNewName()
{
    string name = "UIElement_" + to_string(m_IDCounter);
    m_IDCounter++;
    return name;
}

void UIMgr::SetForDetach(UIElement* _element)
{
    if (find(m_ObjectsToDetach.begin(), m_ObjectsToDetach.end(), _element) != m_ObjectsToDetach.end())
    {
        Modules::log().Warn("UI: Element [%s] already set for detaching.", _element->GetName().c_str());
        return;
    }

    m_ObjectsToDetach.push_back(_element);
}

void UIMgr::SetForDelete(UIElement* _element)
{
    if (find(m_ObjectsToDelete.begin(), m_ObjectsToDelete.end(), _element) != m_ObjectsToDelete.end())
    {
        Modules::log().Warn("UI: Element [%s] already set for deletion.", _element->GetName().c_str());
        return;
    }

    m_ObjectsToDelete.push_back(_element);
}

void UIMgr::DetachFromParent(UIElement* _element)
{
    auto& parent = _element->m_Parent;

    if (parent == nullptr)
    {
        Modules::log().Error("UI: Element [%s] parent is nullptr.", _element->GetName().c_str());
        return;
    }

    auto& elementInParentChildsIt = find(parent->m_Childs.begin(), parent->m_Childs.end(), _element);
    if (*elementInParentChildsIt != _element)
    {
        Modules::log().Error("UI: Element [%s] not finded in parent [%s] childs.", _element->GetName().c_str(), parent->GetName().c_str());
        return;
    }

    parent->m_Childs.erase(elementInParentChildsIt);
    Modules::log().Info("UI: Element [%s] detached from parent [%s].", _element->GetName().c_str(), parent->GetName().c_str());
}

void UIMgr::DeleteUIElement(UIElement* _element)
{
    if (m_FocusedElement == _element)
    {
        m_FocusedElement = nullptr;
    }

    delete _element;
}

//

#pragma region Input functional

On_Mouse_Moved(UIMgr)
{
    assert1(m_RootElement != nullptr);
    m_RootElement->OnMouseMoved(_mousePos);
}

On_Mouse_Pressed(UIMgr)
{
    if (m_FocusedElement != nullptr)
    {
        return m_FocusedElement->OnMouseButtonPressed(_button, _mods, _mousePos);
    }

    //assert1(m_RootElement != nullptr);
    //return m_RootElement->OnMouseButtonPressed(_button, _mods, _mousePos);
}

On_Mouse_Released(UIMgr)
{
    assert1(m_RootElement != nullptr);
    return m_RootElement->OnMouseButtonReleased(_button, _mods, _mousePos);
}

On_Mouse_WScrolled(UIMgr)
{
    assert1(m_RootElement != nullptr);
    return m_RootElement->OnMouseWheel(_yoffset);
}

On_Keyboard_Pressed(UIMgr)
{
    assert1(m_RootElement != nullptr);
    return m_RootElement->OnKeyboardPressed(_key, _scancode, _mods);
}

On_Keyboard_Released(UIMgr)
{
    assert1(m_RootElement != nullptr);
    return m_RootElement->OnKeyboardReleased(_key, _scancode, _mods);
}

On_Character_Printed(UIMgr)
{
    if (m_FocusedElement != nullptr)
    {
        return m_FocusedElement->OnCharInput(_char);
    }

    return false;
}

#pragma endregion
