#pragma once

#include "GameState.h"

class GameState_Empty : public GameState
{
public:
	GameState_Empty();

	bool Init();
	void Destroy();

	//

	void InputPhase(double t, double dt) {}
	void UpdatePhase(double t, double dt) {}
	void RenderPhase(double t, double dt) {}
	void RenderUIPhase(double t, double dt) {}

	//

	V_MOUSE_MOVED{return; };
	V_MOUSE_PRESSED{return false; };
	V_MOUSE_RELEASE{return false; };
	V_MOUSE_WHEEL{return false; };
	V_KEYBD_PRESSED{return false; };
	V_KEYBD_RELEASE{return false; };
	V_CHAR_INPUT{return false; };
};
