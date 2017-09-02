#pragma once

#include "world.h"

class gMapDBRecord;

enum Commands
{
	CMD_NONE,
	CMD_SELECT,
	CMD_LOAD_WORLD,
	CMD_DO_LOAD_WORLD,
	CMD_IN_WORLD
};

class GameState_Menu : public GameState
{
public:
	GameState_Menu() : GameState() {}

	bool Init();
	void Destroy();
	void InputPhase(double t, double dt);
	void UpdatePhase(double t, double dt);
	void RenderPhase(double t, double dt);
	void RenderUIPhase(double t, double dt);

	//

	void OnBtn(gMapDBRecord* _e)
	{
		Debug::Green("Load level %s [%d]", _e->Get_Directory_cstr(), _e->Get_ID());

		_World->GetMap()->PreloadMap(_e);
		cmd = CMD_SELECT;
		window->Detach();
	}

	bool LoadWorld(cvec3 _pos);

	//

	V_MOUSE_MOVED;
	V_MOUSE_PRESSED;
	V_MOUSE_RELEASE;
	V_MOUSE_WHEEL;
	V_KEYBD_PRESSED;
	V_KEYBD_RELEASE;
	V_CHAR_INPUT{return false; }

private:
	// Camera moving
	bool enableFreeCamera;
	bool cameraSprint;
	vec2 lastMousePos;

	UIWindow* window;

	Commands cmd;

	bool minimapActive;

	Model *backgroundModel;
	float mt;

	void randBackground();
};
