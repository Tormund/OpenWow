#pragma once

class InputListener;

class Input : public Module {
public:
	DEF_MODULE(Input, OW_CORE_DLL_API);

	OW_CORE_DLL_API void AddInputListener(InputListener* _inputListener);
	OW_CORE_DLL_API void DeleteInputListener(InputListener* _inputListener);

	OW_CORE_DLL_API vec2 GetMouse() const { return mousePos; }
	OW_CORE_DLL_API bool IsKeyPressed(int key) { return keyState[key]; }
	OW_CORE_DLL_API bool IsMousePressed(int button) { return mouseButtonState[button]; }

	// GLFW Connector
	OW_CORE_DLL_API void MousePositionCallback(cvec2 _mousePos);
	OW_CORE_DLL_API void MouseCallback(int button, int action, int mods);
	OW_CORE_DLL_API void MouseScrollCallback(int yoffset);
	OW_CORE_DLL_API void KeyboardCallback(int key, int scancode, int action, int mods);
	OW_CORE_DLL_API void CharCallback(uint32_t _char);

private:
	bool* keyState;
	bool* mouseButtonState;
	vec2 mousePos;

	vector<InputListener*> inputListeners;
};

#define _Input Input::instance()
