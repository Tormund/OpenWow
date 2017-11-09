#pragma once

#include "InputKeys.h"

class Input
{
public:
    static bool Init();
    static void Destroy();

    static vec2 GetMouse() { return mousePos; }
    static bool IsKeyPressed(int key) { return keyState[key]; }
    static bool IsMousePressed(int button) { return mouseButtonState[button]; }

	// GLFW Connector
    static void MousePositionCallback(cvec2 _mousePos);
    static void MouseCallback(int button, int action, int mods);
    static void MouseScrollCallback(int yoffset);
    static void KeyboardCallback(int key, int scancode, int action, int mods);
    static void CharCallback(uint32 _char);

private:
	static bool keyState[OW_KEYSCOUNT];
    static bool mouseButtonState[OW_MOUSEBUTTONSCOUNT];
    static vec2 mousePos;
};
