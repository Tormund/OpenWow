#pragma once

#include "DBCFile.h"

__DBC_DEF_BEGIN(gLiquidTypeDB)

ADD_STRING(Name, 1);

ADD_TYPE_VALUE(uint32_t, Type, 3);			// 0: Water, 1: Ocean, 2: Magma, 3: Slime

ADD_TYPE_VALUE(uint32_t, LiquidMaterialID, 14);

ADD_STRING(Texture_FileName_Pattern, 15);
ADD_STRING(Texture_ProceduralName, 15);
ADD_STRING(Bump_FileName , 17);

__DBC_DEF_END