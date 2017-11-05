#pragma once

#include <algorithm>
#include <functional>
#include <fstream>
#include <cmath>
#include <typeinfo>
#include <memory>
using namespace std;

#ifndef WIN32_LEAN_AND_MEAN
#   define WIN32_LEAN_AND_MEAN 1
#endif

#ifndef NOMINMAX
#   define NOMINMAX
#endif

#include <windows.h>

// Macros
#include "Debug.h"
#include "Macros.h"

// Common types
#include "Types.h"
#include "Consts.h"

// General
#include "Random.h"

// Additional types
#include "Function.h"
#include "FunctionWA.h"
#include "FunctionSA.h"
//#include "Timer.h"

// Utils
#include "Utils.h"
