#pragma once

// STL TYPES
#include <algorithm>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <cstdlib>
#include <typeinfo>
#include <memory>
using namespace std;

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// GLEW
#include <GL/glew.h>

// GLFW
#define GLFW_DLL
#include <GLFW/glfw3.h>

// GLFX
#include <GL/glfx.h>

// Core
#include <owCore.h>
#pragma comment(lib, "owCore.lib")

// MAIN
#define OW_RENDER_DLL_API __declspec(dllexport)
#include "common.h"
