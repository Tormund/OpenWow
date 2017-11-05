#include "stdafx.h"

// General
#include "UIWoWMinimapSelector.h"

UIWoWMinimapSelector::UIWoWMinimapSelector() : base() {}

UIWoWMinimapSelector::~UIWoWMinimapSelector()
{

}

void UIWoWMinimapSelector::Init(cvec2 _position, cvec2 _size, Texture* _texture)
{
    m_MinimapImage = new Image(_texture);

    base::Init(_position, m_MinimapImage);
}

void UIWoWMinimapSelector::OnRenderUI()
{
    base::OnRenderUI();
}