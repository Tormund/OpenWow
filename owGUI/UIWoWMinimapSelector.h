#pragma once

// Common
#include "UIButton.h"

class UIWoWMinimapSelector : public UIButton
{
    typedef UIButton base;

public:
    UIWoWMinimapSelector();
    virtual ~UIWoWMinimapSelector();

    void Init(cvec2 _position, cvec2 _size, Texture* _texture);

    virtual void Render();

private:
    Image* m_MinimapImage;
    Function_WA<vec2>* m_OnClickFunction;
};