#pragma once

class InputListener;

class GameState : public InputListener {
public:
	GameState() : inited(false) {};

	virtual bool Init() = 0;
	virtual void Destroy() = 0;
	virtual void InputPhase(double t, double dt) = 0;
	virtual void UpdatePhase(double t, double dt) = 0;
	virtual void RenderPhase(double t, double dt) = 0;
	virtual void RenderUIPhase(double t, double dt) = 0;

	bool IsInited() const { return inited; }

protected:
	bool inited;
};
