#pragma once

class Font;
class UIMgr;
class UIWindow;

class UIElement : public InputListener
{
	friend UIMgr;
public:
	 UIElement();
	virtual ~UIElement();

	//

	 void Init(cvec2 _position, cvec2 _size, Image* _image, Color _color = COLOR_EMPTY);

	// Name functional

	 void SetName(cstring _newName);
	inline string GetName() const { return name; }

	// Childs & parent functional

	 void Attach(UIWindow* _parent);
	 void Detach();
	 void Delete();
	inline UIWindow* GetParent() const { return parent; }

	// Enable & Disable functional

	virtual void Enable();
	virtual void Disable();
	inline bool IsEnabled() const { return !disabled; }
	inline bool IsDisabled() const { return disabled; }

	// Mouse hover functional

	 bool CheckMouseHover();
	inline bool IsMouseHover() const { return mouseHover; }

	// Position & size functional

	 vec2 GetPosition() const;
	inline vec2 GetSize() const { return size; }

	// Text functional

	inline void ShowText() { textEnable = true; }
	inline void HideText() { textEnable = false; }
	 void SetText(cstring _text);
	inline void SetTextOffset(cvec2 _textOffset) { textOffset = _textOffset; }
	inline void SetTextAlignW(TextAlignW _textAlignW) { textAlignW = _textAlignW; }
	inline void SetTextAlignH(TextAlignH _textAlignH) { textAlignH = _textAlignH; }
	 void SetTextFont(Font* _font);
	inline string GetText() const { return text; };

	// Common functional

	virtual void Update();
	virtual void Render();

	virtual void OnMouseLeaved() {};

protected:
	vec2 position;
	vec2 size;
	Image* image;
	Color color;

private:
	string name;
	UIWindow* parent;
	bool disabled;

	bool mouseHover;

protected:
	bool textEnable;
	string text;
	Font* textFont;
	vec2 textOffset;
	TextAlignW textAlignW;
	TextAlignH textAlignH;
};
