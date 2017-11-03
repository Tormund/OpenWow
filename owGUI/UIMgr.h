#pragma once

class Module;
class InputListenerObject;
class UIElement;
class UIWindow;

class UIMgr : public UpdatableObject, public RenderableUIObject, public Module, public InputListenerObject
{
public:
	DEF_MODULE(UIMgr);

	//

	void Attach(UIElement* _element);

	// Common functional

	void Update(double t, double dt) override;
	void RenderUI() override;

	// Focus

	inline UIElement* GetFocus() const { return focusedElement; }
	inline void SetFocus(UIElement* _element) { focusedElement = _element; }

	// Input functional

	V_MOUSE_MOVED;
	V_MOUSE_PRESSED;
	V_MOUSE_RELEASE;
	V_MOUSE_WHEEL;
	V_KEYBD_PRESSED;
	V_KEYBD_RELEASE;
	V_CHAR_INPUT;

private:
	string GetNewName();
	void SetForDetach(UIElement* _element);
	void SetForDelete(UIElement* _element);
	void DetachFromParent(UIElement* _element);
	void DeleteUIElement(UIElement* _element);

private:
	vec2 screenSize;
	unsigned long long idCounter;
	UIWindow* baseWindow;

	vector<UIElement*> objectsToDetach;
	vector<UIElement*> objectsToDelete;
	UIElement* focusedElement;

    //

    friend UIElement;
    friend UIWindow;
};

#define _UIMgr UIMgr::instance()
