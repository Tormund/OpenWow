#pragma once

class InputListener;

class GameState : public GameModule, public InputListener
{
public:
	GameState() : m_inited(false) {};

	virtual bool Init() = 0;
	virtual void Destroy() = 0;

	bool IsInited() const { return m_inited; }

protected:
	bool m_inited;
};
