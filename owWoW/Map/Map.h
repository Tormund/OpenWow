#pragma once

// Includes
#include "../DBC__Storage.h"
#include "Map_Tile.h"

struct WDT_MPHD_Flags
{
	uint32 Flag_GlobalWMO : 1;  // Use global map object definition.
	uint32 Flag_UseMCCV : 1;    // Use vertex shading (ADT.MCNK.MCCV)
    uint32 Flag_8bitMCAL : 1;   // Decides whether to use _env terrain shaders or not: funky and if MCAL has 4096 instead of 2048(?)
    uint32 Flag_Unk0 : 1;       // Disables something. No idea what. Another rendering thing. Someone may check all them in wild life..
    uint32 Flag_HasMCLV : 1;    // vertexBufferFormat = PNC2. (adds second color: ADT.MCNK.MCLV)
	uint32 Flag_FlipGround : 1; // Flips the ground display upside down to create a ceiling (Cataclysm)
	uint32 : 26;
};

struct WDT_MAIN_Flags
{
	uint32 Flag_HasTerrain : 1;
	uint32 Flag_IsOcean : 1;
	uint32 Flag_IsLoaded : 1;
	uint32 : 29;
};

class DBC_MapRecord;

class Map
{
	CLASS_INSTANCE(Map);

	Map();
	~Map();

	void CreateMapArrays();
    vector<uint16> GenarateMapArray(uint16 _holes = 0);
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

	WMOInstance* GetGlobalWMOInstance() { return globalWMO; }
	WMOPlacementInfo* GetGlobalWMOPlacementInfo() { return globalWMOplacementInfo; }

	bool MapHasTiles() { return m_IsTileBased; }
	bool MapHasGlobalWMO() { return globalWMOExists; }

	const WDT_MPHD_Flags& GetMapFlag() { return m_Flag; }

	uint32 GetMinimap() { return minimap; }

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
	uint32 lowrestiles[C_TilesInMap][C_TilesInMap];
	uint32 minimap;
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
    vector<uint16> m_DefaultMapStrip;
    vector<uint16> m_LowResMapStrip;
};

#define _Map Map::instance()

#include "Map.inl"