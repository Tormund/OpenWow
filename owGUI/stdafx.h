#pragma once

// GLFW
#define GLFW_DLL
#include <GLFW/glfw3.h>

#include <owCore.h>
#pragma comment(lib, "owCore.lib")

#include <owRender.h>
#pragma comment(lib, "owRender.lib")

#define OW_GUI_DLL_API __declspec(dllexport)