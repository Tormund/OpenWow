#pragma once

#include "DBC__File.h"

DBC_DEF_BEGIN(DBC_LiquidType)

__DBC_STRING(Name, 2);
__DBC_TVALUE(uint32, Flags, 3);
__DBC_TVALUE(uint32, Type, 4);			// 0: Water, 1: Ocean, 2: Magma, 3: Slime

// 5 	SoundEntries.dbc irefid
// 6    Spell.dbc irefID

__DBC_REF_ID(DBC_LiquidMaterial, LiquidMaterialID, 15);

__DBC_STRING(TextureName, 16);
__DBC_STRING(TextureProceduralName, 17);
__DBC_STRING(TextureBump, 18);
__DBC_STRING(TextureUnknown, 19);

DBC_DEF_END