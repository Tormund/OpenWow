#pragma once

#include "DBCFile.h"

DBC_DEF_BEGIN(DBC_LightSkybox)

__DBC_STRING(Filename, 2);
__DBC_TVALUE(uint32_t, Flags, 3);

DBC_DEF_END