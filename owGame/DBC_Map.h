#pragma once

#include "DBC__File.h"

DBC_DEF_BEGIN(DBC_Map)

__DBC_STRING(Directory, 2);								// string
__DBC_TVALUE(uint32, InstanceType, 3);				// uint (0-none, 1-party, 2-raid, 4-arena, >=5-none)
__DBC_TVALUE(uint32, Flags, 4);						// uint

__DBC_TVALUE(uint32, MapType, 5);					// uint (1-normal, 2-test, 3-internal)

__DBC_TVALUE(uint32, IsBattleground, 6);			// uint (0,1)

__DBC_LOCSTR(Name, 7);

__DBC_REF_ID(DBÑ_AreaTable, AreaTableID, 8);				// uint32

__DBC_LOCSTR(MapDescriptionAllicane, 9);			// loc
__DBC_LOCSTR(MapDescriptionHorde, 10);				// loc

__DBC_TVALUE(uint32, LoadingScreenID, 11);			// uint [LoadingScreen]
__DBC_TVALUE(float_t, MinimapIconScale, 12);			// float 

__DBC_REF_ID(DBC_Map, ParentMapID, 13);				// int

__DBC_TVALUE(float_t, CoordinateX, 14);				// float
__DBC_TVALUE(float_t, CoordinateY, 15);				// float

__DBC_TVALUE(int32, TimeOfDayOverride, 16);			// int
__DBC_TVALUE(float_t, ExpansionID, 17);				// float
__DBC_TVALUE(uint32, MaxPlayers, 19);				// uint

__DBC_TVALUE(int32, PhaseParentMapID, 20);			// int

DBC_DEF_END