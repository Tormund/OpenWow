#pragma once

#include "UpdatableObject.h"
class RenderableUIObject;
class InputListenerObject;

class GameState : public UpdatableObject, public RenderableUIObject, public InputListenerObject
{
public:
	GameState() : m_inited(false) {};

	virtual bool Init() = 0;
	virtual void Destroy() = 0;

    virtual void InputPhase(double t, double dt) {};
    virtual void Render(double t, double dt) {};

	bool IsInited() const { return m_inited; }

protected:
	bool m_inited;
};
