#include "stdafx.h"

// General
#include "UIElement.h"

// Additional
#include "UIMgr.h"
#include "UIWindow.h"

UIElement::UIElement()
{
	// Internal
	name = _UIMgr->GetNewName();
	parent = nullptr;
	disabled = false;
	mouseHover = false;

	// General
	position = VECTOR_ZERO;
	size = VECTOR_ZERO;
	image = nullptr;
	color = COLOR_EMPTY;

	// Text
	textEnable = true;
	text = "<empty>";
	textFont = _FontsMgr->GetMainFont();
	if (textFont != nullptr)
	{
		textOffset = vec2(6, textFont->GetHeight());
	}
	textAlignW = TextAlignW::TEXT_ALIGNW_LEFT;
	textAlignH = TextAlignH::TEXT_ALIGNH_CENTER;

	Debug::Info("UI: [%s] created.", name.c_str());
}

UIElement::~UIElement()
{
	if (image != nullptr)
	{
		delete image;
	}

	Debug::Info("UI: [%s] destroyed.", name.c_str());
}

void UIElement::Init(cvec2 _position, cvec2 _size, Image* _image, Color _color)
{
	position = _position;
	size = _size;
	image = _image;
	color = _color;
}

// Name functional

void UIElement::SetName(cstring _newName)
{
	if (_newName.empty())
		return;

	name = _newName;
}

// Parent & childs functional

void UIElement::Attach(UIWindow* _parent)
{
	if (parent != nullptr)
	{
		Debug::Error("UIElement: Can't attach element, because element already have a parent!");
		return;
	}

	_parent->childs.push_back(this);
	this->parent = _parent;
}

void UIElement::Detach()
{
	/*if (parent != nullptr) {
		for (auto chIt = parent->childs.begin(); chIt != parent->childs.end(); ++chIt)
			if (*chIt == this) {
				Debug::Info("UI: Element [%s] deleted from parent.", (*chIt)->GetName().c_str());
				parent->childs.erase(chIt);
				return;
			}
	}*/
	_UIMgr->SetForDetach(this);
}

void UIElement::Delete()
{
	_UIMgr->SetForDelete(this);
}

// Enable / disable functional

void UIElement::Enable()
{
	disabled = false;
}

void UIElement::Disable()
{
	disabled = true;
}

// Mouse hover functional

bool UIElement::CheckMouseHover()
{
	if (size == VECTOR_ZERO)
	{
		mouseHover = false;
		return false;
	}

	if (Utils::PointInRectangle(_Input->GetMouse(), GetPosition(), GetPosition() + size))
		mouseHover = true;
	else
		mouseHover = false;

	return mouseHover;
}

// Position & size functional

vec2 UIElement::GetPosition() const
{
	if (parent != nullptr)
		return parent->GetPosition() + position;
	return position;
}

// Text functional

void UIElement::SetText(cstring _text)
{
	text = _text;
}

void UIElement::SetTextFont(Font* _font)
{
	if (_font == nullptr)
		return;

	textFont = _font;
}

// Common functional

void UIElement::Update()
{}

void UIElement::Render()
{
	// Render debug
	if (IsMouseHover())
		_Render->RenderRectangle(GetPosition(), size, false, COLOR_BLUE);

	// Render me
	if (size != VECTOR_ZERO)
		if (image != nullptr)
			_Render->RenderImage(GetPosition(), image, size);
		else if (color != COLOR_EMPTY)
			_Render->RenderRectangle(GetPosition(), size, true, color);

	// Render text
	if (textEnable)
	{
		_Render->RenderText(GetPosition() + textOffset, text, textAlignW, textAlignH, textFont);
	}
}
