#pragma once

// Includes
#include "InputListener.h"
#include "InputKeys.h"

class Input : public Module
{
public:
	DEF_MODULE(Input);

	void AddInputListener(InputListener* _inputListener);
	void DeleteInputListener(InputListener* _inputListener);

	vec2 GetMouse() const { return mousePos; }
	bool IsKeyPressed(int key) { return keyState[key]; }
	bool IsMousePressed(int button) { return mouseButtonState[button]; }

	// GLFW Connector
	void MousePositionCallback(cvec2 _mousePos);
	void MouseCallback(int button, int action, int mods);
	void MouseScrollCallback(int yoffset);
	void KeyboardCallback(int key, int scancode, int action, int mods);
	void CharCallback(uint32 _char);

private:
	bool* keyState;
	bool* mouseButtonState;
	vec2 mousePos;

	vector<InputListener*> inputListeners;
};

#define _Input Input::instance()
