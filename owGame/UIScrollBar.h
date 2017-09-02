#pragma once

#include "UIElement.h"

class UIScrollBar : public UIElement {
	typedef UIElement base;
public:
	UIScrollBar();
	~UIScrollBar();

	OW_GAME_DLL_API void Init(cvec2 _position, cvec2 _size);

	//

	int Data() const { return static_cast<int>(value * 100.0); };

	//

	void Render();

	V_MOUSE_MOVED;
	virtual void OnMouseLeaved();
	V_MOUSE_PRESSED;
	V_MOUSE_RELEASE;
	V_MOUSE_WHEEL;

private:
	double value;

	vec2 begunokPoint; // begunok render point
	int begunokYOffset;
	vec2 begunokSize;

	bool begunokSelected; // mouse on begunok
	bool begunokPressed; // mouse pressed

	vec2 begunokBounds;
	int begunokBoundsSize;

	Image* begunokImage;
};
