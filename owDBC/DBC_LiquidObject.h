#pragma once

#include "DBCFile.h"

DBC_DEF_BEGIN(DBC_LiquidObject)

__DBC_TVALUE(float, flowDirection, 2);
__DBC_TVALUE(float, flowSpeed, 3);
__DBC_REF_ID(DBC_LiquidType, liquidTypeID, 4);
__DBC_TVALUE(uint32, fishable, 5);
__DBC_TVALUE(uint32, reflection, 6);

DBC_DEF_END
