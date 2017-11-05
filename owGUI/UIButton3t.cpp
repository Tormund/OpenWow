#include "stdafx.h"

// General
#include "UIButton3t.h"

UIButton3t::UIButton3t() : base() {}

UIButton3t::~UIButton3t()
{
	for (int i = 0; i < 4; i++)
		delete buttonsImages[i];
	m_Image = nullptr;
}

void UIButton3t::Init(cvec2 _position, cstring _textureName)
{
	auto buttonTexture = _TexturesMgr->Add(_textureName);
	auto buttonSize = vec2(buttonTexture->GetSize().x, buttonTexture->GetSize().y / 4);

    for (int i = 0; i < 4; i++)
    {
        buttonsImages[i] = new Image(buttonTexture, vec2(0, buttonSize.y * i), buttonSize);
    }

	//

	base::Init(_position, buttonsImages[0]);
}

void UIButton3t::OnRenderUI()
{
	switch (buttonStatus)
	{
		case BUTTON_NORMAL:
		m_Image = buttonsImages[0];
		break;

		case BUTTON_PRESSED:
		m_Image = buttonsImages[1];
		break;

		case BUTTON_HOVER:
		m_Image = buttonsImages[2];
		break;

		case BUTTON_DISABLED:
		m_Image = buttonsImages[3];
		break;
	}

	base::OnRenderUI();
}
