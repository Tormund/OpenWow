#pragma once

class GameState_MDXViewer : public GameState
{
public:
	GameState_MDXViewer() : GameState() {}

	bool Init();
	void Destroy();
	void InputPhase(double t, double dt);
	void UpdatePhase(double t, double dt);
	void Render(double t, double dt);
	void RenderUI(double t, double dt);

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

	MDX* currentModel;
	float mt;

	void randBackground();
};
