#pragma once

class InputListener;

class GameState : public InputListener
{
public:
	GameState() : m_inited(false) {};

	virtual bool Init() = 0;
	virtual void Destroy() = 0;

	virtual void InputPhase(double t, double dt) = 0;
	virtual void UpdatePhase(double t, double dt) = 0;
	virtual void Render(double t, double dt) = 0;
	virtual void RenderUI(double t, double dt) = 0;

	bool IsInited() const { return m_inited; }

protected:
	bool m_inited;
};
