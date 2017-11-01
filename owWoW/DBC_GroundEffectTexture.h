#pragma once

#include "DBC__File.h"

DBC_DEF_BEGIN(DBC_GroundEffectTexture)

__DBC_TARRAY(uint32, Doodads, 2, 4);
__DBC_TARRAY(int32, Weights, 6, 4);
__DBC_TVALUE(uint32, Amount, 10);
__DBC_TVALUE(uint32, TerrainType, 11);

DBC_DEF_END