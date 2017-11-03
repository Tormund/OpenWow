#pragma once

enum Commands2
{
	CMD_NONE2,
	CMD_SELECT2
};

class GameState_Menu : public GameState
{
public:
	GameState_Menu() : GameState() {}

	bool Init();
	void Destroy();
    void InputPhase(double t, double dt);
	void Update(double t, double dt) override;
	void Render(double t, double dt);
	void RenderUI() override;

	//

    void OnBtn(DBC_MapRecord* _e);

	bool LoadWorld(cvec3 _pos);

	//

    V_MOUSE_MOVED;
    V_MOUSE_PRESSED;
    V_MOUSE_RELEASE;

    V_KEYBD_PRESSED;

private:
	UIWindow* window;
    Texture* m_MinimapTexture;
    Image* m_MinimapImage;
    UIElement* m_MinimapUI;

	Commands2 cmd;

	MDX *backgroundModel;
	float mt;

	void randBackground();



    // Camera moving
    bool enableFreeCamera;
    bool cameraSprint;
    bool cameraSlow;
    vec2 lastMousePos;
};
