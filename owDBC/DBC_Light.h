#pragma once

#include "DBCFile.h"

__DBC_DEF_BEGIN(gLightDB)
ADD_TYPE_VALUE(uint32_t, Map, 1);
ADD_TYPE_VALUE(float_t, PositionX, 2);		// float
ADD_TYPE_VALUE(float_t, PositionY, 3);		// float
ADD_TYPE_VALUE(float_t, PositionZ, 4);		// float
ADD_TYPE_VALUE(float_t, RadiusInner, 5);	// float
ADD_TYPE_VALUE(float_t, RadiusOuter, 6);	// float
ADD_TYPE_VALUE(uint32_t, DataIDs, 7);		// uint[8]
__DBC_DEF_END