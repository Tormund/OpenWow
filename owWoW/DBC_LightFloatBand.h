#pragma once

#include "DBC__File.h"

DBC_DEF_BEGIN(DBC_LightFloatBand)

__DBC_TVALUE(uint32, Count, 2);
__DBC_TARRAY(uint32, Times, 3, 16);
__DBC_TARRAY(float, Values, 19, 16);

DBC_DEF_END