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
	CLASS_INSTANCE2(Map);

	Map();
	~Map();

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
    void Render_DEBUG();
	//

	void enterTile(int x, int z);
	MapTile* LoadTile(int x, int z);
	void ClearCache();
	uint32 getAreaID();

public: // Getters
	string GetFolder() { return m_MapFolder; }
	DBC_MapRecord* GetDBCMap() { return m_DBC_Map; }

	WMOInstance* GetGlobalWMOInstance() { return m_GlobalWMO; }
	WMOPlacementInfo* GetGlobalWMOPlacementInfo() { return m_GlobalWMOPlacementInfo; }

	bool MapHasTiles() { return m_IsTileBased; }
	bool MapHasGlobalWMO() { return m_GlobalWMOPlacementInfo != nullptr; }

	const WDT_MPHD_Flags& GetMapFlag() { return m_Flag; }

	uint32 GetMinimap() { return minimap; }

	int GetCurrentX() { return currentTileX; }
	int GetCurrentZ() { return currentTileZ; }

	bool IsOutOfBounds() const { return m_IsOnInvalidTile; }
	void SetOutOfBounds(bool _value) { m_IsOnInvalidTile = _value; }

private:
	bool IsTileInCurrent(MapTile* _mapTile);

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
	MapTile* current[C_RenderedTiles][C_RenderedTiles];

    int currentTileX, currentTileZ;
	bool                       m_IsOnInvalidTile;

private: // WMOs
	string                     m_GlobalWMOName;
	WMOPlacementInfo*          m_GlobalWMOPlacementInfo;
	WMOInstance*               m_GlobalWMO;

	vector<string>             m_LowResolutionWMOsNames;
	vector<WMOPlacementInfo*>  m_LowResolutionWMOsPlacementInfo;
	vector<WMOInstance*>       m_LowResolutionWMOs;
};

#define _Map Map::instance()

inline bool IsBadTileIndex(int i, int j)
{
    if (i < 0)
    {
        return true;
    }

    if (j < 0)
    {
        return true;
    }

    if (i >= C_TilesInMap)
    {
        return true;
    }

    if (j >= C_TilesInMap)
    {
        return true;
    }

    return false;
}

inline bool IsGoodTileIndex(int i, int j)
{
    return (!IsBadTileIndex(i, j));
}