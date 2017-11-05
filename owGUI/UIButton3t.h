#pragma once

#include "UIButton.h"

class UIButton3t : public UIButton
{
    typedef UIButton base;
public:
    UIButton3t();
    virtual ~UIButton3t();

    void Init(cvec2 _position, cstring _textureName);

    virtual void OnRenderUI() override;

protected:
    Image* buttonsImages[4];

private:
    string textureName;
};
