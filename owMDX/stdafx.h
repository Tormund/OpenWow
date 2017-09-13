#pragma once

// GLEW
#include <GL/glew.h>

// GLFW
#define GLFW_DLL
#include <GLFW/glfw3.h>

// Core
#include <owCore.h>
#pragma comment(lib, "owCore.lib")

// Render utils
#include <owRender.h>
#pragma comment(lib, "owRender.lib")

// Shared
#include <owShared.h>
#pragma comment(lib, "owShared.lib")

#define OW_MDX_DLL_API __declspec(dllexport)