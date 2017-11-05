#pragma once

#include "UIWindow.h"

class UIWoWWindow : public UIWindow
{
    typedef UIWindow base;

public:
    void Init(cvec2 _position, cvec2 _size, Image* _image, Color _color = COLOR_EMPTY);

private:
    Image* m_Corners[4];
    Image* m_Sides[4];
};