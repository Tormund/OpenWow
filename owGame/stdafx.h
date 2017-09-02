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

#include <owGUI.h>
#pragma comment(lib, "owGUI.lib")

#define OW_GAME_DLL_API __declspec(dllexport)