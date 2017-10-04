#pragma once

#include "DBCFile.h"

DBC_DEF_BEGIN(DBC_LightFloatBand)

__DBC_TVALUE(uint32_t, Count, 2);
__DBC_TARRAY(uint32_t, Times, 3, 16);
__DBC_TARRAY(float, Values, 19, 16);

DBC_DEF_END