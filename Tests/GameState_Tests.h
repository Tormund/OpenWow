#pragma once

class GameState_Tests : public GameState
{
public:
	GameState_Tests() : GameState() {}

	bool Init() override;

    //

    void Input(double t, double dt) override;
	void Update(double t, double dt) override;
	void Render(double t, double dt);
	void RenderUI() override;

	//

	//

    V_MOUSE_MOVED;

    V_MOUSE_PRESSED;
    V_MOUSE_RELEASE;

    V_KEYBD_PRESSED;

private:
    UIElement* m_ElementTest;
};
