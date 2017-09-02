#pragma once

// GLM


#include "Types.h"
#include "Consts.h"

// Types
#include "File.h"
#include "Color.h"
#include "Rect.h"
//#include "Vector21.h"
//#include "Vector31.h"

// Common classes
#include "Function.h"
#include "Module.h"
#include "Random.h"
#include "Utils.h"
#include "Debug.h"
#include "Manager.h"

inline void rotate(float x0, float y0, float *x, float *y, float angle) {
	float xa = *x - x0;
	float ya = *y - y0;
	*x = xa * cosf(angle) - ya * sinf(angle) + x0;
	*y = xa * sinf(angle) + ya * cosf(angle) + y0;
}
