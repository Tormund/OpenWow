#pragma once

class Module;
class InputListenerObject;
class UIElement;

class UIMgr : public UpdatableObject, public RenderableUIObject, public Module, public InputListenerObject
{
public:
	DEF_MODULE(UIMgr);

    void Update(double t, double dt) override;
    void RenderUI() override;

	//

	void AttachToRoot(UIElement* _element);
    void DetachFromRoot(UIElement* _element, bool _checkChilds = false);

    void AttachElementToParent(UIElement* _element, UIElement* _parent);

    // Root element

    UIElement* GetRootElement() const { return m_RootElement; }
    void SetRootElement(UIElement* _element) { m_RootElement = _element; }

	// Focus

	UIElement* GetFocus() const { return m_FocusedElement; }
    void SetFocus(UIElement* _element);

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
	uint32              m_IDCounter;
    UIElement*          m_RootElement;
    UIElement*          m_FocusedElement;

	vector<UIElement*>  m_ObjectsToDetach;
	vector<UIElement*>  m_ObjectsToDelete;
	
    //

    friend UIElement;
};

#define _UIMgr UIMgr::instance()
