#pragma once

#include "DBCFile.h"

__DBC_DEF_BEGIN(gMapDB)

ADD_STRING(Directory, 1);								// string
ADD_TYPE_VALUE(uint32_t, InstanceType, 2);				// uint (0-none, 1-party, 2-raid, 4-arena, >=5-none)
ADD_TYPE_VALUE(uint32_t, Flags, 3);						// uint

ADD_TYPE_VALUE(uint32_t, MapType, 4);					// uint (1-normal, 2-test, 3-internal)

ADD_TYPE_VALUE(uint32_t, IsBattleground, 5);			// uint (0,1)

ADD_LOCALE_STRING(Name, 6);								// loc
ADD_TYPE_VALUE(uint32_t, AreaTableId, 7);				// uint32_t
ADD_LOCALE_STRING(MapDescriptionAllicane, 8);			// loc
ADD_LOCALE_STRING(MapDescriptionHorde, 9);				// loc

ADD_TYPE_VALUE(uint32_t, LoadingScreenID, 10);			// uint [LoadingScreen]
ADD_TYPE_VALUE(float_t, MinimapIconScale, 11);			// float 

ADD_TYPE_VALUE(int32_t, ParentMapID, 12);				// int

ADD_TYPE_VALUE(float_t, CoordinateX, 13);				// float
ADD_TYPE_VALUE(float_t, CoordinateY, 14);				// float

ADD_TYPE_VALUE(int32_t, TimeOfDayOverride, 15);			// int
ADD_TYPE_VALUE(float_t, ExpansionID, 16);				// float
ADD_TYPE_VALUE(uint32_t, MaxPlayers, 18);				// uint

ADD_TYPE_VALUE(int32_t, PhaseParentMapID, 19);			// int

__DBC_DEF_END