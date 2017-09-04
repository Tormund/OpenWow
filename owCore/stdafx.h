#pragma once

// STL TYPES
#include <algorithm>
#include <fstream>
#include <cmath>
#include <typeinfo>
#include <memory>
using namespace std;

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <owLibMPQ.h>
#pragma comment(lib, "owLibMPQ.lib")

#define OW_CORE_DLL_API __declspec(dllexport)
#include "common.h"
