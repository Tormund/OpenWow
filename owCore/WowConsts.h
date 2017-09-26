#pragma once

// Tiles
const uint32_t C_TilesInMap = 64;
const uint32_t C_RenderedTiles = 5;
const uint32_t C_TilesCacheSize = ((C_RenderedTiles + 3) * (C_RenderedTiles + 3));

const float C_TileSize = 533.33333f;
const float C_ChunkSize = C_TileSize / 16.0f;
const float C_UnitSize = C_ChunkSize / 8.0f;
const float C_ZeroPoint = 32.0f * C_TileSize;

// Tile chunk
const uint32_t C_ChunksInTile = 16;
const uint32_t C_MapBufferSize = 9 * 9 + 8 * 8;

// Liquid
const float LQ_DEFAULT_TILESIZE = (C_ChunkSize / 8.0f);

// World
const float detail_size = 8.0f;