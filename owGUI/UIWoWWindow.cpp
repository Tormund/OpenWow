#include "stdafx.h"

// General
#include "UIWoWWindow.h"

void UIWoWWindow::Init(cvec2 _position, cvec2 _size, Image * _image, Color _color)
{
    Texture* cornersTexture = _TexturesMgr->Add("Interface//DialogFrame//DialogFrame-Corners.blp");

    for (uint8 i = 0; i < 4; i++)
    {
        m_Corners[i] = new Image(cornersTexture, vec2(32.0f, 32.0f));
    }

    // TOL_LEFT
    m_Corners[0]->SetStart(vec2(0.0f, 0.0f));

    // TOL_RIGHT
    m_Corners[1]->SetStart(vec2(32.0f, 0.0f));

    // BOT_RIGHT
    m_Corners[2]->SetStart(vec2(32.0f, 32.0f));

    // BOT_LEFT
    m_Corners[3]->SetStart(vec2(0.0f, 32.0f));

    base::Init(_position, _size, nullptr);
}
