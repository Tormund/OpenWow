#pragma once

// Includes
#include "MapTile.h"
#include "Map_GlobalWMOs.h"

// Defines
#define TILES_IN_MAP 64
#define RENDERED_TILES 3
#define MAPTILECACHESIZE ((RENDERED_TILES + 1) * (RENDERED_TILES + 1))

class Map
{
public:
	Map();
	~Map();

	void PreloadMap(gMapDBRecord* _map);
	void LoadLowTerrain();

	void Tick();

	void RenderSky();
	void RenderLowResTiles();
	void RenderTiles();
	void RenderObjects();
	void RenderModels();
	void RenderWater();

	void enterTile(int x, int z);
	MapTile* LoadTile(int x, int z);
	uint32_t getAreaID();

public: // Getters
	string GetPath() { return path; }
	gMapDBRecord* GetTemplate() { return templateMap; }
	Map_GlobalWMOs* GetMapWMOs() { return &m_Map_GlobalWMOs; }

	bool IsBigAlpha() { return m_BigAlpha; }

	bool MapHasTerrain() { return mapHasTerrain; }
	bool MapHasGlobalWMO() { return m_Map_GlobalWMOs.IsGlobalWMOExists(); }

	size_t GetTilesCount() { return tilesCount; }
	GLuint GetMinimap() { return minimap; }

	int GetCurrentX() { return currentTileX; }
	int GetCurrentZ() { return currentTileZ; }

	bool IsOutOfBounds() const { return outOfBounds; }

private:
	Map_GlobalWMOs m_Map_GlobalWMOs;

private:
	string path;
	gMapDBRecord* templateMap;

	bool m_BigAlpha;

private:
	bool mapHasTerrain;
	size_t tilesCount;
	bool m_TileExists[64][64];
	bool m_TileIsWater[64][64];
	GLuint lowrestiles[64][64];
	GLuint minimap;
	MapTile* maptilecache[MAPTILECACHESIZE];
	int currentTileX, currentTileZ;
	int enteredTileX, enteredTileZ;
	MapTile* current[RENDERED_TILES][RENDERED_TILES];
	bool outOfBounds;
};

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

	if (i >= TILES_IN_MAP)
	{
		return true;
	}

	if (j >= TILES_IN_MAP)
	{
		return true;
	}

	return false;
}

inline bool IsGoodTileIndex(int i, int j)
{
	return (!IsBadTileIndex(i, j));
}