#pragma once

// Common
#include "DBC__File.h"

DBC_DEFINE(DBÑ_AreaTable);

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



// My files
#include "DBC_AreaTable.h"

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



class GroundEffectTextureDB : public DBCFile<Record>
{
public:
	GroundEffectTextureDB() :
		DBCFile("GroundEffectTexture.dbc")
	{}

	/// Fields
	static const size_t ID = 0;				// uint
	static const size_t Doodads = 1;		// uint[4]
	static const size_t Weights = 5;		// uint[4]
	static const size_t Amount = 9;			// uint
	static const size_t TerrainType = 10;	// uint
};

class GroundEffectDoodadDB : public DBCFile<Record>
{
public:
	GroundEffectDoodadDB() :
		DBCFile("GroundEffectDoodad.dbc")
	{}

	/// Fields
	static const size_t ID = 0;				// uint
	static const size_t Filename = 1;		// string
};

 void OpenDBs();

 extern GroundEffectDoodadDB gGroundEffectDoodadDB;
 extern GroundEffectTextureDB gGroundEffectTextureDB;
