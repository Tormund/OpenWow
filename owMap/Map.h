#pragma once

// Includes
#include "Map_Tile.h"

class Map
{
	CLASS_INSTANCE(Map);

	Map();
	~Map();

	void CreateMapArrays();
	void InitGlobalsWMOs();

	void PreloadMap(gMapDBRecord* _map);
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
	uint32_t getAreaID();

public: // Getters
	string GetPath() { return path; }
	gMapDBRecord* GetTemplate() { return templateMap; }

#ifdef WMO_INCL
	WMOInstance* GetGlobalWMOInstance() { return globalWMO; }
	WMOPlacementInfo* GetGlobalWMOPlacementInfo() { return globalWMOplacementInfo; }
#endif

	bool IsBigAlpha() { return m_BigAlpha; }

	bool MapHasTerrain() { return mapHasTerrain; }
	bool MapHasGlobalWMO() { return globalWMOExists; }

	uint32_t GetTilesCount() { return tilesCount; }
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

	int16_t* GetLowResolutionIndexes()
	{
		return mapstrip;
	}

	int16_t* GetHighResolutionIndexes()
	{
		return mapstrip2;
	}

private:
	bool IsTileInCurrent(MapTile* _mapTile);

	// Fields

private:
	string path;
	gMapDBRecord* templateMap;

	bool m_BigAlpha;

private:
	bool mapHasTerrain;
	uint32_t tilesCount;
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
	uint32_t lowResolutionWMOsCount;
	vector<string> lowResolutionWMOsNames;
	vector<WMOPlacementInfo*> lowResolutionWMOsplacementInfo;
	vector<WMOInstance*> lowResolutionWMOs;

private: // Index buffer
	vec2 dataDetail[C_MapBufferSize];
	vec2 dataAlpha[C_MapBufferSize];
	int16_t* mapstrip;
	int16_t* mapstrip2;

public: // Consts
	const uint32_t stripsize = 8 * 18 + 7 * 2;
	const uint32_t stripsize2 = 16 * 18 + 7 * 2 + 8 * 2;
};

#define _Map Map::instance()

#include "Map.inl"