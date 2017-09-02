#pragma once

#include "DBCFile.h"

__DBC_DEF_BEGIN(gLightSkyboxDB)

ADD_STRING(Filename, 1);
ADD_TYPE_VALUE(uint32_t, Flags, 2);

__DBC_DEF_END