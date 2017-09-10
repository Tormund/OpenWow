#pragma once

#include "GameState.h"

class GameState_Empty : public GameState
{
public:
	GameState_Empty();

	bool Init() override;
	void Destroy() override;

	//

	void InputPhase(double t, double dt)  override {}
	void UpdatePhase(double t, double dt)  override {}
	void Render(double t, double dt)  override {}
	void RenderUI(double t, double dt)  override {}

	//

	V_MOUSE_MOVED{return;};
	V_MOUSE_PRESSED{return false;};
	V_MOUSE_RELEASE{return false;};
	V_MOUSE_WHEEL{return false;};
	V_KEYBD_PRESSED{return false;};
	V_KEYBD_RELEASE{return false;};
	V_CHAR_INPUT{return false;};
};
