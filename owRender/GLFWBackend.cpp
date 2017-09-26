#include "stdafx.h"

// General
#include "GLFWBackend.h"

// Additional
#include "Render.h"
//#define GLFW_EXPOSE_NATIVE_WIN32
//#include <GLFW/glfw3native.h>

// Error callback
void GLFWErrorCallback(int error, const char* description) { Debug::Error("GLFW[]: Error [%d] (%s).", error, description); }

// Resize callback
void GLFWFramebufferCallback(GLFWwindow* _window, int _width, int _height) { _GLFW->SetWindowSize(_width, _height); }

// Input callbacks
void GLFWMousePositionCallback(GLFWwindow* window, double xpos, double ypos) { _Input->MousePositionCallback(vec2(static_cast<int>(xpos), static_cast<int>(ypos))); }
void GLFWMouseCallback(GLFWwindow* window, int button, int action, int mods) { _Input->MouseCallback(button, action, mods); }
void GLFWMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) { _Input->MouseScrollCallback(static_cast<int>(yoffset)); }
void GLFWKeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) { _Input->KeyboardCallback(key, scancode, action, mods); }
void GLFWCharCallback(GLFWwindow* window, unsigned int _char) { _Input->CharCallback(_char); }

//---------------------------------------------------------

bool GLFWBackend::Init()
{
	// Error callback
	glfwSetErrorCallback(GLFWErrorCallback);

	// Loading glfw libary
	if (!glfwInit())
	{
		Debug::Error("GLFW[]: Error while init GLFW!");
		return false;
	}
	Debug::Info("GLFW[]: Version [%s].", glfwGetVersionString());

	//

	// Set window options
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	//glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	//

	// Monitor
	primaryMonitor = glfwGetPrimaryMonitor();
	const char* primaryMonitorName = glfwGetMonitorName(primaryMonitor);
	Debug::Info("GLFW[]: Primary monitor name [%s]", primaryMonitorName);

	// Videomode contain settings
	const GLFWvidmode* primaryMonitorMode = glfwGetVideoMode(primaryMonitor);
	Debug::Info("GLFW[]: Primary monitor resolution [%d, %d]", primaryMonitorMode->width, primaryMonitorMode->height);

	// Physical size in mm
	int32_t primaryMonitorPhysicalWidth, primaryMonitorPhysicalHeight;
	glfwGetMonitorPhysicalSize(primaryMonitor, &primaryMonitorPhysicalWidth, &primaryMonitorPhysicalHeight);
	Debug::Info("GLFW[]: Primary monitor physical size [%d, %d]", primaryMonitorPhysicalWidth, primaryMonitorPhysicalHeight);

	//

	// Create GLFW window
	window = glfwCreateWindow(_Settings->windowSizeX, _Settings->windowSizeY, "Default window title.", nullptr, nullptr);

	// Move window to center
	uint32_t windowPositionX = (primaryMonitorMode->width / 2) - (_Settings->windowSizeX / 2);
	uint32_t windowPositionY = (primaryMonitorMode->height / 2) - (_Settings->windowSizeY / 2);
	glfwSetWindowPos(window, windowPositionX, windowPositionY);
	Debug::Info("GLFW[]: Window position [%d, %d]", windowPositionX, windowPositionY);

	//nativeWindow = glfwGetWin32Window(window);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// Time
	glfwSetTime(0.0);

	// Resize callback
	glfwSetFramebufferSizeCallback(window, GLFWFramebufferCallback);

	//

	// Loading glew libary
	//glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		Debug::Error("GLFW[]: Error while init GLEW! [%s]", glewGetErrorString(err));
		return false;
	}
	Debug::Info("GLFW[]: GLEW version [%s]", glewGetString(GLEW_VERSION));

	//

	// Input
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 0);
	glfwSetMouseButtonCallback(window, GLFWMouseCallback);
	glfwSetCursorPosCallback(window, GLFWMousePositionCallback);
	glfwSetScrollCallback(window, GLFWMouseScrollCallback);
	glfwSetKeyCallback(window, GLFWKeyboardCallback);
	glfwSetCharCallback(window, GLFWCharCallback);

	return true;
}

void GLFWBackend::Destroy()
{
	glfwTerminate();
}

bool GLFWBackend::SwapWindowBuffers()
{
	glfwSwapBuffers(window);
	glfwPollEvents();

	return !glfwWindowShouldClose(window);
}

//

void GLFWBackend::SetWindowSize(int32_t _width, int32_t _height)
{
	glfwSetWindowSize(window, _width, _height);
	_Render->OnWindowResized(_width, _height);
}

void GLFWBackend::SetWindowTitle(cstring _title)
{
	glfwSetWindowTitle(window, _title.c_str());
}

void GLFWBackend::ShowCursor()
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void GLFWBackend::HideCursor()
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void GLFWBackend::SetMousePosition(cvec2 _mousePosition)
{
	glfwSetCursorPos(window, static_cast<double>(_mousePosition.x), static_cast<double>(_mousePosition.y));
}