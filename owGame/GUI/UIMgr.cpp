#include "../stdafx.h"

// Common
#include "UIElement.h"
#include "UIWindow.h"

// General
#include "UIMgr.h"

bool UIMgr::Init()
{
	screenSize = Modules::config().GetWindowSize();
	idCounter = 0;
	baseWindow = new UIWindow();
	baseWindow->Init(VECTOR_ZERO, screenSize, nullptr);

	focusedElement = nullptr;

	return RenderableUIObject::Register(500);
}

void UIMgr::Destroy()
{
	baseWindow->Delete();

	RenderableUIObject::Unregister();
}

void UIMgr::Attach(UIElement* _element)
{
	_element->Attach(baseWindow);
}

void UIMgr::Update()
{
	// Detach from parent
	for (auto it = objectsToDetach.begin(); it != objectsToDetach.end(); )
	{
		DetachFromParent(*it);

		it = objectsToDetach.erase(it);
	}

	// Delete
	for (auto it = objectsToDelete.begin(); it != objectsToDelete.end(); )
	{
		DetachFromParent(*it);

		if (auto thisAsUIWindow = dynamic_cast<UIWindow*>(*it))
			thisAsUIWindow->DeleteChilds();

		Modules::log().Info("UI: Element [%s] deleted", (*it)->GetName().c_str());
		DeleteUIElement(*it);

		it = objectsToDelete.erase(it);
	}
}

void UIMgr::RenderUI()
{
	baseWindow->Render();
}

//

string UIMgr::GetNewName()
{
	string _name = "UIElement" + to_string(idCounter);
	idCounter++;
	return _name;
}

void UIMgr::SetForDetach(UIElement* _element)
{
	if (find(objectsToDetach.begin(), objectsToDetach.end(), _element) != objectsToDetach.end())
	{
		Modules::log().Warn("UI: Element [%s] already set for detaching.", _element->GetName().c_str());
		return;
	}

	objectsToDetach.push_back(_element);
}

void UIMgr::SetForDelete(UIElement* _element)
{
	if (find(objectsToDelete.begin(), objectsToDelete.end(), _element) != objectsToDelete.end())
	{
		Modules::log().Warn("UI: Element [%s] already set for deletion.", _element->GetName().c_str());
		return;
	}

	objectsToDelete.push_back(_element);
}

void UIMgr::DetachFromParent(UIElement* _element)
{
	auto& parent = _element->parent;

	if (parent == nullptr)
	{
		Modules::log().Error("UI: Element [%s] parent is nullptr.", _element->GetName().c_str());
		return;
	}

	auto& elementInParentChildsIt = find(parent->childs.begin(), parent->childs.end(), _element);

	if (*elementInParentChildsIt != _element)
	{
		Modules::log().Error("UI: Element [%s] not finded in parent [%s] childs.", _element->GetName().c_str(), parent->GetName().c_str());
		return;
	}

	parent->childs.erase(elementInParentChildsIt);
	Modules::log().Info("UI: Element [%s] detached from parent [%s].", _element->GetName().c_str(), parent->GetName().c_str());

	parent = nullptr;
}

void UIMgr::DeleteUIElement(UIElement* _element)
{
	if (focusedElement == _element)
		focusedElement = nullptr;

	delete _element;
}

//

#pragma region Input functional

MOUSE_MOVED_(UIMgr)
{
	baseWindow->OnMouseMoved(_mousePos);
}

MOUSE_PRESSED(UIMgr)
{
	if (baseWindow->CheckMouseHover())
		return baseWindow->OnMouseButtonPressed(_button, _mods, _mousePos);

	return false;
}

MOUSE_RELEASE(UIMgr)
{
	return baseWindow->OnMouseButtonReleased(_button, _mods, _mousePos);
}

MOUSE_WHEEL(UIMgr)
{
	if (baseWindow->CheckMouseHover())
		return baseWindow->OnMouseWheel(_yoffset);

	return false;
}

KEYBD_PRESSED(UIMgr)
{
	return baseWindow->OnKeyboardPressed(_key, _scancode, _mods);
}

KEYBD_RELEASE(UIMgr)
{
	return baseWindow->OnKeyboardReleased(_key, _scancode, _mods);
}

CHAR_INPUT(UIMgr)
{
	if (focusedElement != nullptr)
		return focusedElement->OnCharInput(_char);

	return false;
}

#pragma endregion
