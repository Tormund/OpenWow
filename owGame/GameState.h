#pragma once

class GameStateManager;
class UIWindow;

class GameState : public UpdatableObject, public RenderableUIObject, public InputListenerObject
{
public:
    GameState();

	virtual bool Init();
	virtual void Destroy();

    virtual bool Set();
    virtual void Unset();

    virtual void Render(double t, double dt) {};

	bool IsInited() const { return m_IsInited; }
    bool IsCurrent() const { return m_IsCurrent; }

protected:
	bool      m_IsInited;
    bool      m_IsCurrent;
    UIWindow* m_Window;

    //

    friend GameStateManager;
};
