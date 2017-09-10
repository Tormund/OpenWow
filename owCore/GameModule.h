#pragma once

__interface GameModule
{
	void InputPhase(double t, double dt);
	void UpdatePhase(double t, double dt);
	void Render(double t, double dt);
	void RenderUI(double t, double dt);
};