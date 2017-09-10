#pragma once

class Module;

class GLFWBackend : public Module
{
public:
	DEF_MODULE(GLFWBackend, OW_RENDER_DLL_API);

	//

	OW_RENDER_DLL_API bool SwapWindowBuffers();

	//

	OW_RENDER_DLL_API void SetWindowSize(int32_t _width, int32_t _height);
	OW_RENDER_DLL_API void SetWindowTitle(cstring _title);

	OW_RENDER_DLL_API void ShowCursor();
	OW_RENDER_DLL_API void HideCursor();

	OW_RENDER_DLL_API void SetMousePosition(cvec2 _mousePosition);

	inline const double GetTime() const { return glfwTime; }

private:
	GLFWmonitor* primaryMonitor;
	GLFWwindow* window;

	double glfwTime;
};

#define _GLFW GLFWBackend::instance()
