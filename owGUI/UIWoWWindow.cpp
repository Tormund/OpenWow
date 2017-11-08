#include "stdafx.h"

// General
#include "UIWoWWindow.h"

void UIWoWWindow::Init(cvec2 _position, cvec2 _size)
{
    Texture* texture = _TexturesMgr->Add("Interface/DialogFrame/UI-DialogBox-Border.blp");
    for (uint8 i = 0; i < 8; i++)
    {
        m_Images[i] = new Image(texture, vec2(i * 32.0f, 0.0f), vec2(C_ImgSize, C_ImgSize));
    }

    m_Images[IMG_TOP]->RotateCoords(Image::CoordsRotation::Coords_ROT_90);
    m_Images[IMG_BOT]->RotateCoords(Image::CoordsRotation::Coords_ROT_90);

    m_Background = _TexturesMgr->Add("Interface/DialogFrame/UI-DialogBox-Background-Dark.blp");

    base::Init(_position, _size, nullptr);
}

void UIWoWWindow::OnRenderUI()
{
    _Render->RenderTexture(GetPosition() + (C_ImgSize / 4.0f), m_Background, GetSize() - (C_ImgSize / 2.0f));

    // Top left
    _Render->RenderImage(GetPosition(), m_Images[IMG_CORNER_TOP_LEFT]);

    // Top 
    _Render->RenderImage(GetPosition() + vec2(C_ImgSize, 0.0f),
                         m_Images[IMG_TOP],
                         vec2(GetSize().x - 2.0f * C_ImgSize, C_ImgSize));

    // Top Right
    _Render->RenderImage(GetPosition() + vec2(GetSize().x, 0.0f) - vec2(C_ImgSize, 0.0f), m_Images[IMG_CORNER_TOP_RIGHT]);

    // Right
    _Render->RenderImage(GetPosition() + vec2(GetSize().x - C_ImgSize, C_ImgSize),
                         m_Images[IMG_RIGHT],
                         vec2(C_ImgSize, GetSize().y - 2.0f * C_ImgSize));

    // Bot Right
    _Render->RenderImage(GetPosition() + GetSize() - C_ImgSize, m_Images[IMG_CORNER_BOT_RIGHT]);

    // Bot
    _Render->RenderImage(GetPosition() + vec2(C_ImgSize, GetSize().y - C_ImgSize),
                         m_Images[IMG_BOT],
                         vec2(GetSize().x - 2.0f * C_ImgSize, C_ImgSize));

    // Bot Left
    _Render->RenderImage(GetPosition() + vec2(0.0f, GetSize().y) - vec2(0.0f, C_ImgSize), m_Images[IMG_CORNER_BOT_LEFT]);

    // Left
    _Render->RenderImage(GetPosition() + vec2(0.0f, C_ImgSize),
                         m_Images[IMG_LEFT],
                         vec2(C_ImgSize, GetSize().y - 2.0f * C_ImgSize));

    base::OnRenderUI();
}
