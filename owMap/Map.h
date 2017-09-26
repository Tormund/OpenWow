#pragma once

// Includes
#include "MapTile.h"
#include "Map_GlobalWMOs.h"

class Map
{
public:
	Map();
	~Map();

	//

	void PreloadMap(gMapDBRecord* _map);
	void LoadLowTerrain();

	//

	void Tick();

	//

	void RenderSky();
	void RenderLowResTiles();
	void RenderTiles(MapTilePass* pass);
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
	Map_GlobalWMOs* GetMapWMOs() { return &m_Map_GlobalWMOs; }
#endif

	bool IsBigAlpha() { return m_BigAlpha; }

	bool MapHasTerrain() { return mapHasTerrain; }
	bool MapHasGlobalWMO() { return m_Map_GlobalWMOs.IsGlobalWMOExists(); }

	uint32_t GetTilesCount() { return tilesCount; }
	GLuint GetMinimap() { return minimap; }

	int GetCurrentX() { return currentTileX; }
	int GetCurrentZ() { return currentTileZ; }

	bool IsOutOfBounds() const { return outOfBounds; }
	void SetOutOfBounds(bool _value) { outOfBounds = _value; }

private:
	bool IsTileInCurrent(MapTile* _mapTile);

private:
	string path;
	gMapDBRecord* templateMap;
	Map_GlobalWMOs m_Map_GlobalWMOs;

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
};

//

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