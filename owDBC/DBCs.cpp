#include "stdafx.h"

// General
#include "DBCs.h"

// Additional
#include "DBC_Light.h"
#include "DBC_LightSkyBox.h"
#include "DBC_LiquidMaterial.h"
#include "DBC_LiquidType.h"
#include "DBC_LoadingScreen.h"
#include "DBC_Map.h"

LightIntBandDB gLightIntBandDB;
LightFloatBandDB gLightFloatBandDB;
GroundEffectDoodadDB gGroundEffectDoodadDB;
GroundEffectTextureDB gGroundEffectTextureDB;

void OpenDBs()
{
	gLightDB.Open();
	gLightSkyboxDB.Open();
	gLiquidMaterialDB.Open();
	gLiquidTypeDB.Open();
	gLoadingScreensDB.Open();
	gMapDB.Open();




	gLightIntBandDB.Open();
	gLightFloatBandDB.Open();
	gGroundEffectDoodadDB.Open();
	gGroundEffectTextureDB.Open();

}
