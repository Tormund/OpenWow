#pragma once

#define On_Mouse_Entered(c)      void c::OnMouseEntered()
#define On_Mouse_Entered_V       virtual void OnMouseEntered() override;

#define On_Mouse_Leaved(c)       void c::OnMouseLeaved()
#define On_Mouse_Leaved_V        virtual void OnMouseLeaved() override;

class Font;
class UIMgr;
class UIWindow;

class UIElement
{
public:
	UIElement(uint32 _DeepAdding = 0);
	virtual ~UIElement();

	//

	void Init(cvec2 _position, cvec2 _size, Image* _image, Color _color = COLOR_EMPTY);
    void Init(cvec2 _position, cvec2 _size, Texture* _texture, Color _color = COLOR_EMPTY);

	// Name functional

	void SetName(cstring _newName);
	inline string GetName() const { return m_Name; }

	// Childs & parent functional

	void AttachTo(UIElement* _parent = nullptr);
	void Detach();
	void Delete();

    // Parent & childs functional

    UIElement* GetParent() const { return m_Parent; }
    vector<UIElement*>& GetChilds() { return m_Childs; }
    uint32 GetChildsCount() const { return static_cast<uint32>(m_Childs.size()); }
	
	// Show & Hide functional

	virtual void Show();
	virtual void Hide();
	bool IsHiden() const { return m_IsHided; }

	// Mouse hover functional

	bool CheckSelection(cvec2 _mousePos);
	bool IsSelected() const { return m_IsSelected; }

	// Position & size functional

	vec2 GetPosition() const;
	vec2 GetSize() const { return m_Size; }

	// Text functiona

	void ShowText() { m_IsTextEnable = true; }
	void HideText() { m_IsTextEnable = false; }
	void SetText(cstring _text);
	void SetTextOffset(cvec2 _textOffset) { m_TextOffset = _textOffset; }
	void SetTextAlignW(TextAlignW _textAlignW) { m_TextAlignW = _textAlignW; }
	void SetTextAlignH(TextAlignH _textAlignH) { m_TextAlignH = _textAlignH; }
	void SetTextFont(Font* _font);
	string GetText() const { return m_Text; };

	// Common functional

	virtual void OnUpdate();
	virtual void OnRenderUI();

    virtual void OnMouseEntered();
    virtual void OnMouseMoved(cvec2 _mousePos);
    virtual void OnMouseLeaved();
    virtual bool OnMouseButtonPressed(int _button, int _mods, cvec2 _mousePos);
    virtual bool OnMouseButtonReleased(int _button, int _mods, cvec2 _mousePos);
    virtual bool OnMouseWheel(int _yoffset);
    virtual bool OnKeyboardPressed(int _key, int _scancode, int _mods);
    virtual bool OnKeyboardReleased(int _key, int _scancode, int _mods);
    virtual bool OnCharInput(uint32 _char) { return false; }
	
private:
    void Update();
    void RenderUI();

    /*void MouseMoved(cvec2 _mousePos);
    void MouseLeaved();
    bool MouseButtonPressed(int _button, int _mods, cvec2 _mousePos);
    bool MouseButtonReleased(int _button, int _mods, cvec2 _mousePos);
    bool MouseWheel(int _yoffset);
    bool KeyboardPressed(int _key, int _scancode, int _mods);
    bool KeyboardReleased(int _key, int _scancode, int _mods);
    bool CharInput(uint32 _char);*/

    void DeleteChilds();

protected:
	vec2        m_Position;
	vec2        m_Size;
	Image*      m_Image;
    Texture*    m_Texture;
	Color       m_Color;

private:
	string      m_Name;
    uint32      m_Deep;
    uint32      m_DeepAdding;
    UIElement*  m_Parent;
    vector<UIElement*> m_Childs;

    // internal states
	bool        m_IsHided;
    bool        m_InputDisabled;
	bool        m_IsSelected;

protected:
	bool        m_IsTextEnable;
	string      m_Text;
	Font*       m_Font;
	vec2        m_TextOffset;
	TextAlignW  m_TextAlignW;
	TextAlignH  m_TextAlignH;

    //

    friend UIMgr;
};
