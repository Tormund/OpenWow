#pragma once

// STL TYPES
#include <algorithm>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <cstdlib>
#include <memory>
#include <ctime>
using namespace std;

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define NOMINMAX
#include <winerror.h>

// INCLUDES
#include <GL/glew.h>

#define GLFW_DLL
#include <GLFW/glfw3.h>

#include <owCore.h>
#pragma comment(lib, "owCore.lib")

#include <owRender.h>
#pragma comment(lib, "owRender.lib")

#include <owGame.h>
#pragma comment(lib, "owGame.lib")

#include <owDBC.h>
#pragma comment(lib, "owDBC.lib")
