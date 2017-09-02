#pragma once

#include "DBCFile.h"


class LightIntBandDB : public DBCFile<Record>
{
public:
	LightIntBandDB() :
		DBCFile("DBFilesClient\\LightIntBand.dbc")
	{}

	/// Fields
	static const size_t ID = 0;				// uint
	static const size_t Entries = 1;		// uint
	static const size_t Times = 2;			// uint
	static const size_t Values = 18;		// uint
};

class LightFloatBandDB : public DBCFile<Record>
{
public:
	LightFloatBandDB() :
		DBCFile("DBFilesClient\\LightFloatBand.dbc")
	{}

	/// Fields
	static const size_t ID = 0;				// uint
	static const size_t Entries = 1;		// uint
	static const size_t Times = 2;			// uint
	static const size_t Values = 18;		// float
};

class GroundEffectTextureDB : public DBCFile<Record>
{
public:
	GroundEffectTextureDB() :
		DBCFile("DBFilesClient\\GroundEffectTexture.dbc")
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
		DBCFile("DBFilesClient\\GroundEffectDoodad.dbc")
	{}

	/// Fields
	static const size_t ID = 0;				// uint
	static const size_t Filename = 1;		// string
};

OW_DBC_DLL_API void OpenDBs();

OW_DBC_DLL_API extern LightIntBandDB gLightIntBandDB;
OW_DBC_DLL_API extern LightFloatBandDB gLightFloatBandDB;
OW_DBC_DLL_API extern GroundEffectDoodadDB gGroundEffectDoodadDB;
OW_DBC_DLL_API extern GroundEffectTextureDB gGroundEffectTextureDB;
//OW_DBC_DLL_API extern LiquidTypeDB gLiquidTypeDB;
