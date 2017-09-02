#pragma once

struct Image;

class UIButton3t : public UIButton {
	typedef UIButton base;
public:
	OW_GAME_DLL_API UIButton3t();
	virtual ~UIButton3t();

	OW_GAME_DLL_API void Init(cvec2 _position, cstring _textureName);

	virtual void Render();

protected:
	Image* buttonsImages[4];

private:
	string textureName;
};
