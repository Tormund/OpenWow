#pragma once

// Includes
#include "../DBC__Storage.h"
#include "Map_Tile.h"

class DBC_MapRecord;

class Map
{
	CLASS_INSTANCE(Map);

	Map();
	~Map();

	void CreateMapArrays();
	void InitGlobalsWMOs();

	void PreloadMap(DBC_MapRecord* _map);
	void LoadLowTerrain();
	void UnloadMap();

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
	string GetPath() { return path; }
	DBC_MapRecord* GetTemplate() { return templateMap; }

#ifdef WMO_INCL
	WMOInstance* GetGlobalWMOInstance() { return globalWMO; }
	WMOPlacementInfo* GetGlobalWMOPlacementInfo() { return globalWMOplacementInfo; }
#endif

	bool IsBigAlpha() { return m_BigAlpha; }

	bool MapHasTerrain() { return (m_TilesCount > 0); }
	bool MapHasGlobalWMO() { return globalWMOExists; }

	uint32 GetTilesCount() { return m_TilesCount; }
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
	string path;
	DBC_MapRecord* templateMap;

	bool m_BigAlpha;

private:
	uint32 m_TilesCount;
	bool m_TileExists[C_TilesInMap][C_TilesInMap];
	bool m_TileIsWater[C_TilesInMap][C_TilesInMap];
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