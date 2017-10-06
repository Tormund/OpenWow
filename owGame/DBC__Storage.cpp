#include "stdafx.h"

// General
#include "DBC__Storage.h"

DBC_LOAD(DBÑ_AreaTable, "AreaTable.dbc")

DBC_LOAD(DBC_Light, "Light.dbc")
DBC_LOAD(DBC_LightFloatBand, "LightFloatBand.dbc")
DBC_LOAD(DBC_LightIntBand, "LightIntBand.dbc")
DBC_LOAD(DBC_LightParams, "LightParams.dbc")
DBC_LOAD(DBC_LightSkybox, "LightSkybox.dbc")

DBC_LOAD(DBC_LiquidMaterial, "LiquidMaterial.dbc")
DBC_LOAD(DBC_LiquidObject, "LiquidObject.dbc")
DBC_LOAD(DBC_LiquidType, "LiquidType.dbc")

DBC_LOAD(DBC_LoadingScreen, "LoadingScreens.dbc")

DBC_LOAD(DBC_Map, "Map.dbc")

GroundEffectDoodadDB gGroundEffectDoodadDB;
GroundEffectTextureDB gGroundEffectTextureDB;

void OpenDBs()
{
	DBÑ_AreaTable.Open();

	DBC_Light.Open();
	DBC_LightFloatBand.Open();
	DBC_LightIntBand.Open();
	DBC_LightParams.Open();
	DBC_LightSkybox.Open();

	DBC_LiquidMaterial.Open();
	DBC_LiquidObject.Open();
	DBC_LiquidType.Open();

	DBC_LoadingScreen.Open();
	DBC_Map.Open();


	gGroundEffectDoodadDB.Open();
	gGroundEffectTextureDB.Open();

}
