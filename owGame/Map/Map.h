#pragma once

// Includes
#include "../DBC__Storage.h"
#include "Map_Tile.h"

struct WDT_MPHD_Flags
{
	uint32 ONLY_GLOBAL_WMO : 1;				// Use global map object definition.
	uint32 ALL_MCNK_HAS_MCCV : 1;				// Use vertex shading (ADT.MCNK.MCCV)
	uint32 ALL_MCNK_MCAL_BIGALPHA : 1;	// Decides whether to use _env terrain shaders or not: funky and if MCAL has 4096 instead of 2048(?)
	uint32 Map_Disable_Something : 1;			// Disables something. No idea what. Another rendering thing. Someone may check all them in wild life..
	uint32 ALL_MCNK_HAS_MCLV : 1;				// vertexBufferFormat = PNC2. (adds second color: ADT.MCNK.MCLV)
	uint32 Map_FlipGroundDisplay : 1;		// Flips the ground display upside down to create a ceiling (Cataclysm)
	uint32 : 26;
};

struct WDT_MAIN_Flags
{
	uint32 HasADT : 1;
	uint32 AllWater : 1;
	uint32 Loaded : 1;
	uint32 : 29;
};

class DBC_MapRecord;

class Map
{
	CLASS_INSTANCE(Map);

	Map();
	~Map();

	void CreateMapArrays();
	void InitGlobalsWMOs();

	void Load_WDT(DBC_MapRecord* _map);
	void Load_WDL();
	void Unload();

	void Tick();

	void RenderSky();
	void RenderLowResTiles();
	void RenderTiles();
	void RenderObjects();
	void RenderModels();
	void RenderWater();

	//

	void enterTile(int x, int z);
	MapTile* LoadTile(int x, int z);
	void ClearCache();
	uint32 getAreaID();

public: // Getters
	string GetFolder() { return m_MapFolder; }
	DBC_MapRecord* GetDBCMap() { return m_DBC_Map; }

#ifdef WMO_INCL
	WMOInstance* GetGlobalWMOInstance() { return globalWMO; }
	WMOPlacementInfo* GetGlobalWMOPlacementInfo() { return globalWMOplacementInfo; }
#endif

	bool MapHasTiles() { return m_IsTileBased; }
	bool MapHasGlobalWMO() { return globalWMOExists; }

	const WDT_MPHD_Flags& GetMapFlag() { return m_Flag; }

	GLuint GetMinimap() { return minimap; }

	int GetCurrentX() { return currentTileX; }
	int GetCurrentZ() { return currentTileZ; }

	bool IsOutOfBounds() const { return outOfBounds; }
	void SetOutOfBounds(bool _value) { outOfBounds = _value; }

	const vec2* GetTextureCoordDetail()
	{
		return &dataDetail[0];
	}

	const vec2* GetTextureCoordAlpha()
	{
		return &dataAlpha[0];
	}

	int16* GetLowResolutionIndexes()
	{
		return mapstrip;
	}

	int16* GetHighResolutionIndexes()
	{
		return mapstrip2;
	}

private:
	bool IsTileInCurrent(MapTile* _mapTile);

	// Fields

private:
	string m_MapFolder;
	DBC_MapRecord* m_DBC_Map;
	WDT_MPHD_Flags m_Flag;

private:
	bool m_IsTileBased;
	WDT_MAIN_Flags m_TileFlag[C_TilesInMap][C_TilesInMap];
	GLuint lowrestiles[C_TilesInMap][C_TilesInMap];
	GLuint minimap;
	MapTile* maptilecache[C_TilesCacheSize];
	int currentTileX, currentTileZ;
	MapTile* current[C_RenderedTiles][C_RenderedTiles];
	bool outOfBounds;

private: // Global wmo
	bool globalWMOExists;
	string globalWMOName;
	WMOPlacementInfo* globalWMOplacementInfo;
	WMOInstance* globalWMO;

private: // Low-resolution WMOs
	uint32 lowResolutionWMOsCount;
	vector<string> lowResolutionWMOsNames;
	vector<WMOPlacementInfo*> lowResolutionWMOsplacementInfo;
	vector<WMOInstance*> lowResolutionWMOs;

private: // Index buffer
	vec2 dataDetail[C_MapBufferSize];
	vec2 dataAlpha[C_MapBufferSize];
	int16* mapstrip;
	int16* mapstrip2;

public: // Consts
	const uint32 stripsize = 8 * 18 + 7 * 2;
	const uint32 stripsize2 = 16 * 18 + 7 * 2 + 8 * 2;
};

#define _Map Map::instance()

#include "Map.inl"