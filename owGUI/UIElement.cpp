#include "stdafx.h"

// General
#include "UIElement.h"

// Additional
#include "UIMgr.h"

UIElement::UIElement(uint32 _DeepAdding)
{
    // Internal
    m_Name = _UIMgr->GetNewName();
    m_Parent = nullptr;
    m_Deep = 0;
    m_DeepAdding = _DeepAdding;

    m_IsHided = false;
    m_InputDisabled = true;
    m_IsSelected = false;

    // General
    m_Position = VECTOR_ZERO;
    m_Size = VECTOR_ZERO;
    m_Image = nullptr;
    m_Texture = nullptr;
    m_Color = COLOR_EMPTY;

    // Text
    m_IsTextEnable = true;
    m_Text = "<empty>";
    m_TextAlignW = TextAlignW::TEXT_ALIGNW_LEFT;
    m_TextAlignH = TextAlignH::TEXT_ALIGNH_CENTER;

    m_Font = _FontsMgr->GetMainFont();
    if (m_Font == nullptr)
    {
        m_TextOffset = vec2(0.0f, 0.0f);
        return;
    }
    m_TextOffset = vec2(6, m_Font->GetHeight());


    //Log::Info("UI: [%s] created.", name.c_str());
}

UIElement::~UIElement()
{
    if (m_Image != nullptr)
    {
        delete m_Image;
    }

    //Log::Info("UI: [%s] destroyed.", name.c_str());
}

void UIElement::Init(cvec2 _position, cvec2 _size, Image* _image, Color _color)
{
    m_Position = _position;
    m_Size = _size;
    m_Image = _image;
    m_Color = _color;
}

void UIElement::Init(cvec2 _position, cvec2 _size, Texture* _texture, Color _color)
{
    m_Position = _position;
    m_Size = _size;
    m_Texture = _texture;
    m_Color = _color;
}

// Name functional

void UIElement::SetName(cstring _newName)
{
    if (_newName.empty())
    {
        return;
    }

    m_Name = _newName;
}

// Parent & childs functional

void UIElement::AttachTo(UIElement* _parent)
{
    _UIMgr->AttachElementToParent(this, _parent);
}

void UIElement::Detach()
{
    _UIMgr->SetForDetach(this);
}

void UIElement::Delete()
{
    _UIMgr->SetForDelete(this);
}

// Show / disable functional

void UIElement::Show()
{
    m_IsHided = false;
}

void UIElement::Hide()
{
    m_IsHided = true;
}

// Mouse hover functional

bool UIElement::CheckSelection(cvec2 _mousePos)
{
    if (m_Size == VECTOR_ZERO)
    {
        m_IsSelected = false;
        return false;
    }

    if (Utils::PointInRectangle(_mousePos, GetPosition(), GetPosition() + GetSize()))
    {
        m_IsSelected = true;
    }
    else
    {
        m_IsSelected = false;
    }

    return m_IsSelected;
}

// Position & size functional

vec2 UIElement::GetPosition() const
{
    if (m_Parent != nullptr)
    {
        return m_Parent->GetPosition() + m_Position;
    }

    return m_Position;
}

// Text functional

void UIElement::SetText(cstring _text)
{
    m_Text = _text;
}

void UIElement::SetTextFont(Font* _font)
{
    assert1(_font);

    m_Font = _font;
}

// Common functional

void UIElement::OnUpdate()
{}

void UIElement::OnRenderUI()
{
    // Render me
    if (m_Size != VECTOR_ZERO)
    {
        if (m_Image)
        {
            _Render->RenderImage(GetPosition(), m_Image, m_Size);
        }
        else if (m_Texture)
        {
            _Render->RenderTexture(GetPosition(), m_Texture, m_Size);
        }
        else if (m_Color != COLOR_EMPTY)
        {
            _Render->RenderRectangle(GetPosition(), m_Size, m_Color);
        }
    }

    // Render text
    if (m_IsTextEnable)
    {
        _Render->RenderText(GetPosition() + m_TextOffset, m_Text, m_TextAlignW, m_TextAlignH, m_Font);
    }
}

// Private functional

void UIElement::Update()
{
    // Recalculate focus
    if (IsSelected())
    {
        _UIMgr->SetFocus(this);
    }

    // Update me
    OnUpdate();

    // Update childs
    for (auto it : m_Childs)
    {
        it->Update();
    }
}

void UIElement::RenderUI()
{
    // Render me
    OnRenderUI();

    // Render childs
    for (auto it : m_Childs)
    {
        it->RenderUI();
    }
}

void UIElement::DeleteChilds()
{
    for (auto chIt = m_Childs.begin(); chIt != m_Childs.end(); )
    {
        UIElement*& child = (*chIt);

        if (child->GetChildsCount() > 0)
        {
            child->DeleteChilds();
        }

        _UIMgr->DeleteUIElement(child);

        chIt = m_Childs.erase(chIt);
    }
}
