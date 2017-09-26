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

	inline const double GetTimeSeconds() const { return glfwTime; }

private:
	GLFWmonitor* primaryMonitor;
	GLFWwindow* window;

	double glfwTime;
};

#define _GLFW GLFWBackend::instance()
