#pragma once

class Module;

class GLFWBackend : public Module
{
public:
	DEF_MODULE_API(GLFWBackend, );

	 bool SwapWindowBuffers();

	 //

	 void SetWindowSize(int32_t _width, int32_t _height);
	 void SetWindowTitle(cstring _title);

	 void ShowCursor();
	 void HideCursor();

	 void SetMousePosition(cvec2 _mousePosition);

private:
	GLFWmonitor* primaryMonitor;
	GLFWwindow* window;
};

#define _GLFW GLFWBackend::instance()
