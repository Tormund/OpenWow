#pragma once

// STL TYPES
#include <algorithm>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <cstdlib>
#include <memory>
using namespace std;

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <owCore.h>
#pragma comment(lib, "owCore.lib")

#define OW_DBC_DLL_API __declspec(dllexport)