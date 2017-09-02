#pragma once

#include "DBCFile.h"

__DBC_DEF_BEGIN(gLoadingScreensDB)

ADD_STRING(Name, 1);
ADD_STRING(FileName, 2);
ADD_TYPE_VALUE(uint32_t, HasWideScreen, 3);

__DBC_DEF_END
