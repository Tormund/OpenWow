#pragma once

// Tiles
const uint32 C_TilesInMap = 64;
const uint32 C_RenderedTiles = 3;
const uint32 C_TilesCacheSize = ((C_RenderedTiles + 3) * (C_RenderedTiles + 3));

const float C_TileSize = 533.33333f;
const float C_ChunkSize = C_TileSize / 16.0f;
const float C_UnitSize = C_ChunkSize / 8.0f;
const float C_ZeroPoint = 32.0f * C_TileSize;

// Tile chunk
const uint32 C_ChunksInTile = 16;
const uint32 C_ChunksInTileGlobal = C_ChunksInTile * C_ChunksInTile;
const uint32 C_MapBufferSize = 9 * 9 + 8 * 8;

// World
const float C_DetailSize = 8.0f;