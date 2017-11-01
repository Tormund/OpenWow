#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

class OpenGLAdapter_GLFW : public OpenGLAdapter
{
public:
	bool Init() override;
	void Destroy() override;

	bool SwapWindowBuffers() override;
	double GetTime() override;

	 //

	void SetWindowSize(int32 _width, int32 _height) override;
	void SetWindowTitle(cstring _title) override;

	void ShowCursor() override;
	void HideCursor() override;

	void SetMousePosition(cvec2 _mousePosition) override;

private:
	GLFWmonitor* primaryMonitor;
	GLFWwindow* window;
};
