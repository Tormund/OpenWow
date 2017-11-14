#pragma once

#include "DBC__File.h"

DBC_DEF_BEGIN(DBC_Light)

__DBC_REF_ID(DBC_Map,  MapID,       2);
__DBC_TVALUE(float_t,  PositionX,   3);
__DBC_TVALUE(float_t,  PositionY,   4);
__DBC_TVALUE(float_t,  PositionZ,   5);
__DBC_TVALUE(float_t,  RadiusInner, 6);
__DBC_TVALUE(float_t,  RadiusOuter, 7);
__DBC_TARRAY(uint32,   Params,      8,    8);

DBC_DEF_END