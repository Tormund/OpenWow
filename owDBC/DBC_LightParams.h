#pragma once

#include "DBCFile.h"

DBC_DEF_BEGIN(DBC_LightParams)

__DBC_TVALUE(uint32_t, m_highlightSky, 2);
__DBC_REF_ID(DBC_LightSkybox, LightSkyboxID, 3);
__DBC_TVALUE(uint32_t, CloudTypeID, 4);
__DBC_TVALUE(float, Glow, 5);
__DBC_TVALUE(float, WaterShallowAlpha, 6);
__DBC_TVALUE(float, WaterDeepAlpha, 7);
__DBC_TVALUE(float, OceanShallowAlpha, 8);
__DBC_TVALUE(float, OceanDeepAlpha, 9);
__DBC_TVALUE(uint32_t, Flags, 10);

DBC_DEF_END