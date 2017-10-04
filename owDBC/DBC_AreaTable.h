#pragma once

#include "DBCFile.h"

DBC_DEF_BEGIN(DBÑ_AreaTable)

__DBC_REF_ID(DBC_Map, MapID, 2);
__DBC_REF_ID(DBÑ_AreaTable, ParentAreaID, 3);

__DBC_TVALUE(int32_t, Bits, 4);
__DBC_TVALUE(uint32_t, Flags, 5);

//__DBC_TVALUE(iRefID, SoundProviderPrefferences, 6);
//__DBC_TVALUE(iRefID, Underwater_SoundProviderPrefferences, 7);
//__DBC_TVALUE(iRefID, SoundAmbience, 8);
//__DBC_TVALUE(iRefID, ZoneMusic, 9);
//__DBC_TVALUE(iRefID, ZoneIntroMusicTable, 10);

__DBC_TVALUE(int32_t, ExplorationLevel, 11);

__DBC_LOCSTR(Name, 12);

DBC_DEF_END