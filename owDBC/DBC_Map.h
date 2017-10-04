#pragma once

#include "DBCFile.h"

DBC_DEF_BEGIN(DBC_Map)

__DBC_STRING(Directory, 2);								// string
__DBC_TVALUE(uint32_t, InstanceType, 3);				// uint (0-none, 1-party, 2-raid, 4-arena, >=5-none)
__DBC_TVALUE(uint32_t, Flags, 4);						// uint

__DBC_TVALUE(uint32_t, MapType, 5);					// uint (1-normal, 2-test, 3-internal)

__DBC_TVALUE(uint32_t, IsBattleground, 6);			// uint (0,1)

__DBC_LOCSTR(Name, 7);

__DBC_REF_ID(DBÑ_AreaTable, AreaTableID, 8);				// uint32_t

__DBC_LOCSTR(MapDescriptionAllicane, 9);			// loc
__DBC_LOCSTR(MapDescriptionHorde, 10);				// loc

__DBC_TVALUE(uint32_t, LoadingScreenID, 11);			// uint [LoadingScreen]
__DBC_TVALUE(float_t, MinimapIconScale, 12);			// float 

__DBC_REF_ID(DBC_Map, ParentMapID, 13);				// int

__DBC_TVALUE(float_t, CoordinateX, 14);				// float
__DBC_TVALUE(float_t, CoordinateY, 15);				// float

__DBC_TVALUE(int32_t, TimeOfDayOverride, 16);			// int
__DBC_TVALUE(float_t, ExpansionID, 17);				// float
__DBC_TVALUE(uint32_t, MaxPlayers, 19);				// uint

__DBC_TVALUE(int32_t, PhaseParentMapID, 20);			// int

DBC_DEF_END