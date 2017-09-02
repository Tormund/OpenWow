#pragma once

// STL TYPES
#include <algorithm>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <cstdlib>
#include <typeinfo>
using namespace std;

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// GLFW
#define GLFW_DLL
#include <GLFW/glfw3.h>

#include <owCore.h>
#pragma comment(lib, "owCore.lib")

#include <owRender.h>
#pragma comment(lib, "owRender.lib")

#define OW_GUI_DLL_API __declspec(dllexport)