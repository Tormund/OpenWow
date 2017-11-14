#pragma once

class GameState_InWorld : public GameState
{
public:
    GameState_InWorld() : GameState() {}

    bool Init() override;
    void Destroy() override;

    //

    bool Set() override;
    void Unset() override;

    //

    void Input(double t, double dt) override;
    void Update(double t, double dt) override;
    void Render(double t, double dt);
    void RenderUI() override;

    void RenderUIDebug();

    //

    V_MOUSE_MOVED;
    V_MOUSE_PRESSED;
    V_MOUSE_RELEASE;
    V_MOUSE_WHEEL;
    V_KEYBD_PRESSED;
    V_KEYBD_RELEASE;

private:
    // Camera moving
    bool enableFreeCamera;
    bool cameraSprint;
    bool cameraSlow;
    vec2 lastMousePos;

    bool minimapActive;
};
