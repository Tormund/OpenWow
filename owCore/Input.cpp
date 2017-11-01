#include "stdafx.h"

// General
#include "Input.h"

Input::Input()
{
	for (int i = 0; i < OW_KEYSCOUNT; i++)
	{
		keyState[i] = false;
	}

	for (int i = 0; i < OW_MOUSEBUTTONSCOUNT; i++)
	{
		mouseButtonState[i] = false;
	}
}

Input::~Input()
{
	inputListeners.clear();
}

void Input::AddInputListener(InputListener* _inputListener)
{
	//assert1(inputListeners.find(_inputListener) != inputListeners.end());

	if (_inputListener != nullptr)
	{
		inputListeners.push_back(_inputListener);
	}
}

void Input::DeleteInputListener(InputListener* _inputListener)
{
	inputListeners.erase(std::remove(inputListeners.begin(), inputListeners.end(), _inputListener), inputListeners.end());
}

//

void Input::MousePositionCallback(cvec2 _mousePos)
{
	mousePos = _mousePos;

	for (auto it = inputListeners.begin(); it != inputListeners.end(); ++it)
	{
		(*it)->OnMouseMoved(mousePos);
	}
}

void Input::MouseCallback(int button, int action, int mods)
{
	if (action == OW_PRESS)
	{
		mouseButtonState[button] = true;
		for (auto it = inputListeners.begin(); it != inputListeners.end(); ++it)
			if ((*it)->OnMouseButtonPressed(button, action, mousePos))
				break;
	}
	else if (action == OW_RELEASE)
	{
		mouseButtonState[button] = false;

		for (auto it = inputListeners.begin(); it != inputListeners.end(); ++it)
			if ((*it)->OnMouseButtonReleased(button, action, mousePos))
				break;
	}
}

void Input::MouseScrollCallback(int yoffset)
{
	for (auto it = inputListeners.begin(); it != inputListeners.end(); ++it)
		if ((*it)->OnMouseWheel(yoffset))
			break;
}

void Input::KeyboardCallback(int key, int scancode, int action, int mods)
{
	if (action == OW_PRESS || action == OW_REPEAT)
	{
		keyState[key] = true;

		for (auto it = inputListeners.begin(); it != inputListeners.end(); ++it)
			if ((*it)->OnKeyboardPressed(key, scancode, mods))
				break;
	}
	else if (action == OW_RELEASE)
	{
		keyState[key] = false;

		for (auto it = inputListeners.begin(); it != inputListeners.end(); ++it)
			if ((*it)->OnKeyboardReleased(key, scancode, mods))
				break;
	}
}

void Input::CharCallback(uint32 _char)
{
	for (auto it = inputListeners.begin(); it != inputListeners.end(); ++it)
		if ((*it)->OnCharInput(_char))
			break;
}
