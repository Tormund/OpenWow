#pragma once

// Common
#include "DBC__File.h"

DBC_DEFINE(DBÑ_AreaTable);

DBC_DEFINE(DBC_GroundEffectDoodad);
DBC_DEFINE(DBC_GroundEffectTexture);

DBC_DEFINE(DBC_Light);
DBC_DEFINE(DBC_LightFloatBand);
DBC_DEFINE(DBC_LightIntBand);
DBC_DEFINE(DBC_LightParams);
DBC_DEFINE(DBC_LightSkybox);

DBC_DEFINE(DBC_LiquidMaterial);
DBC_DEFINE(DBC_LiquidObject);
DBC_DEFINE(DBC_LiquidType);

DBC_DEFINE(DBC_LoadingScreen);
DBC_DEFINE(DBC_Map);
DBC_DEFINE(DBC_WMOAreaTable)


// My files
#include "DBC_AreaTable.h"

#include "DBC_GroundEffectDoodad.h"
#include "DBC_GroundEffectTexture.h"

#include "DBC_Light.h"
#include "DBC_LightFloatBand.h"
#include "DBC_LightIntBand.h"
#include "DBC_LightParams.h"
#include "DBC_LightSkyBox.h"

#include "DBC_LiquidMaterial.h"
#include "DBC_LiquidObject.h"
#include "DBC_LiquidType.h"

#include "DBC_LoadingScreen.h"
#include "DBC_Map.h"
#include "DBC_WMOAreaTable.h"

 void OpenDBs();


class DBC__Storage
{
public:
	bool Init();
	void Destroy();
};
