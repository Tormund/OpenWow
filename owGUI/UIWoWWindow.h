#pragma once

#include "UIWindow.h"

class UIWoWWindow : public UIWindow
{
    typedef UIWindow base;

public:
    void Init(cvec2 _position, cvec2 _size);

    void OnRenderUI() override;

private:
    Texture*  m_Background;
    Image*    m_Images[8];
    enum Images
    {
        IMG_LEFT,
        IMG_RIGHT,
        IMG_TOP,
        IMG_BOT,

        IMG_CORNER_TOP_LEFT,
        IMG_CORNER_TOP_RIGHT,
        IMG_CORNER_BOT_LEFT,
        IMG_CORNER_BOT_RIGHT
    };

    const float C_ImgSize = 32.0f;
};