#pragma once

#include "DBCFile.h"

DBC_DEF_BEGIN(DBC_LiquidType)

__DBC_STRING(Name, 2);
__DBC_TVALUE(uint32, Flags, 3);
__DBC_TVALUE(uint32, Type, 4);			// 0: Water, 1: Ocean, 2: Magma, 3: Slime

// 5 	SoundEntries.dbc irefid
// 6    Spell.dbc irefID

__DBC_REF_ID(DBC_LiquidMaterial, LiquidMaterialID, 15);

__DBC_STRING(Texture_FileName_Pattern, 16);
__DBC_STRING(Texture_ProceduralName, 17);
__DBC_STRING(Bump_FileName, 18);

DBC_DEF_END