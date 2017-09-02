#include "stdafx.h"

// Common
#include "InputListener.h"

// General
#include "Input.h"

// FIXME !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define GLFW_RELEASE                0
#define GLFW_PRESS                  1
#define GLFW_REPEAT                 2
// FIXME !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#define KEYSCOUNT 349
#define MOUSEBUTTONSCOUNT 8

bool Input::Init() {
	// Inits keystates
	keyState = new bool[KEYSCOUNT];
	for (int i = 0; i < KEYSCOUNT; i++)
		keyState[i] = false;

	// Inits mousebuttons states
	mouseButtonState = new bool[MOUSEBUTTONSCOUNT];
	for (int i = 0; i < MOUSEBUTTONSCOUNT; i++)
		mouseButtonState[i] = false;

	return true;
}

void Input::Destroy() {
	delete[] keyState;
	delete[] mouseButtonState;

	inputListeners.clear();
}

void Input::AddInputListener(InputListener* _inputListener) {
	if (_inputListener != nullptr)
		inputListeners.push_back(_inputListener);
}

void Input::DeleteInputListener(InputListener* _inputListener) {
	inputListeners.erase(std::remove(inputListeners.begin(), inputListeners.end(), _inputListener), inputListeners.end());
}

//

void Input::MousePositionCallback(cvec2 _mousePos) {
	mousePos = _mousePos;

	for (auto it = inputListeners.begin(); it != inputListeners.end(); ++it)
		(*it)->OnMouseMoved(mousePos);
}

void Input::MouseCallback(int button, int action, int mods) {
	if (action == GLFW_PRESS) {
		mouseButtonState[button] = true;
		for (auto it = inputListeners.begin(); it != inputListeners.end(); ++it)
			if ((*it)->OnMouseButtonPressed(button, action, mousePos))
				break;
	}
	else if (action == GLFW_RELEASE) {
		mouseButtonState[button] = false;

		for (auto it = inputListeners.begin(); it != inputListeners.end(); ++it)
			if ((*it)->OnMouseButtonReleased(button, action, mousePos))
				break;
	}
}

void Input::MouseScrollCallback(int yoffset) {
	for (auto it = inputListeners.begin(); it != inputListeners.end(); ++it)
		if ((*it)->OnMouseWheel(yoffset))
			break;
}

void Input::KeyboardCallback(int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		keyState[key] = true;

		for (auto it = inputListeners.begin(); it != inputListeners.end(); ++it)
			if ((*it)->OnKeyboardPressed(key, scancode, mods))
				break;
	}
	else if (action == GLFW_RELEASE) {
		keyState[key] = false;

		for (auto it = inputListeners.begin(); it != inputListeners.end(); ++it)
			if ((*it)->OnKeyboardReleased(key, scancode, mods))
				break;
	}
}

void Input::CharCallback(unsigned int _char) {
	for (auto it = inputListeners.begin(); it != inputListeners.end(); ++it)
		if ((*it)->OnCharInput(_char))
			break;
}
