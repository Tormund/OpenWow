#include "stdafx.h"

// General
#include "Map.h"

// Additional
#include "MapChunk.h"
#include "ModelsManager.h"
#include "WMOsManager.h"
#include "World.h"

Map::Map()
{
	mapHasTerrain = false;
	tilesCount = 0;
	memset(m_TileExists, 0, sizeof(m_TileExists));
	memset(m_TileIsWater, 0, sizeof(m_TileIsWater));
	memset(lowrestiles, 0, sizeof(lowrestiles));
	minimap = 0;
	memset(maptilecache, 0, sizeof(maptilecache));
	currentTileX = currentTileZ = -1;
	enteredTileX = enteredTileZ = -1;
	memset(current, 0, sizeof(current));
	outOfBounds = false;
}

Map::~Map()
{
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			if (lowrestiles[i][j] != 0)
			{
				glDeleteLists(lowrestiles[i][j], 1);
			}
		}
	}

	for (int i = 0; i < MAPTILECACHESIZE; i++)
	{
		if (maptilecache[i] != nullptr)
		{
			delete maptilecache[i];
		}
	}

	if (minimap)
	{
		glDeleteTextures(1, &minimap);
	}
}

void Map::PreloadMap(gMapDBRecord* _map)
{
	templateMap = _map;

	Debug::Print("Map[%s]: Id [%d]. Preloading...", templateMap->Get_Directory_cstr(), templateMap->Get_ID());

	path = "World\\Maps\\" + string(templateMap->Get_Directory_cstr()) + "\\";

	// Init main section
	File f(path + templateMap->Get_Directory_cstr() + ".wdt");
	if (!f.Open())
	{
		Debug::Info("Map[%s]: WDT: Error opening.", templateMap->Get_Directory_cstr());
		return;
	}

	char fourcc[5];
	uint32_t size;

	while (!f.IsEof())
	{
		memset(fourcc, 0, 4);
		size = 0;
		f.ReadBytes(fourcc, 4);
		f.ReadBytes(&size, 4);
		flipcc(fourcc);
		fourcc[4] = 0;
		if (size == 0) continue;
		size_t nextpos = f.GetPos() + size;

		if (strcmp(fourcc, "MVER") == 0)
		{
			uint32_t version;
			f.ReadBytes(&version, 4);
			assert3(version == 18, "Version mismatch != 18", std::to_string(version).c_str());
		}
		else if (strcmp(fourcc, "MPHD") == 0)
		{
			enum MPHD_Flags
			{
				Map_No_Terrain = 0x1,				// Use global map object definition.
				Map_VertexPNC = 0x2,				// Use vertex shading (ADT.MCNK.MCCV)
				Map_TerrainShaders_BigAlpha = 0x4,	// Decides whether to use _env terrain shaders or not: funky and if MCAL has 4096 instead of 2048(?)
													//Map_Disable_Something = 0x8			// Disables something. No idea what. Another rendering thing. Someone may check all them in wild life..
													Map_VertexPNC2 = 0x10,				// vertexBufferFormat = PNC2. (adds second color: ADT.MCNK.MCLV)
													Map_FlipGroundDisplay = 0x20,		// Flips the ground display upside down to create a ceiling (Cataclysm)
			};

			// unit32 flags
			// unit32 something
			// unit32 unused[6]

			uint32_t flags;
			f.ReadBytes(&flags, 4);

			m_BigAlpha = ((flags & Map_TerrainShaders_BigAlpha) == Map_TerrainShaders_BigAlpha);
		}
		else if (strcmp(fourcc, "MAIN") == 0) // Map m_TileExists table. Contains 64x64 = 4096 records of 8 bytes each.
		{
			enum MAIN_Flags
			{
				Map_HasADT = 0x01,
				Map_AllWater = 0x02,
				Map_Loaded = 0x04,
			};

			// uint32_t flags
			// uint32_t asyncId

			for (int i = 0; i < 64; i++)
			{
				for (int j = 0; j < 64; j++)
				{
					uint32_t flags;
					f.ReadBytes(&flags, 4);

					if ((flags & Map_HasADT) == Map_HasADT)
					{
						m_TileExists[j][i] = true;
						tilesCount++;
					}

					if ((flags & Map_AllWater) == Map_AllWater)
					{
						m_TileIsWater[j][i] = true;
					}

					uint32_t asyncId;
					f.ReadBytes(&asyncId, 4);
				}
			}
		}
		else if (strcmp(fourcc, "MWMO") == 0) // Global WMO filename
		{
			if (size > 0)
			{
				char* buf = new char[size];
				f.ReadBytes(buf, size);

				string name = string(buf);
				m_Map_GlobalWMOs.SetGlobalWMOName(name);

				delete[] buf;
			}
		}
		else if (strcmp(fourcc, "MODF") == 0)
		{
			size_t globalWMOCount = WMOPlacementInfo::__size;
			assert4(globalWMOCount < 2, "Map has more then 1 global WMO ", templateMap->Get_Directory_cstr(), std::to_string(globalWMOCount).c_str());

			if (globalWMOCount > 0)
			{
				WMOPlacementInfo* placement = new WMOPlacementInfo;
				f.ReadBytes(placement, WMOPlacementInfo::__size);
				m_Map_GlobalWMOs.SetGlobalWMOPlacementInfo(placement);
			}
		}
		else
		{
			Debug::Info("Map[%s]: WDT: Chunks [%s], Size [%d] not implemented.", templateMap->Get_Directory_cstr(), fourcc, size);
		}

		f.Seek(nextpos);
	}

	// Load terrain
	if (tilesCount > 0)
	{
		mapHasTerrain = true;
		LoadLowTerrain();
		Debug::Info("Map[%s]: Is tile-based map.", templateMap->Get_Directory_cstr());
	}
}

void Map::LoadLowTerrain()
{
	File f(path + templateMap->Get_Directory_cstr() + ".wdl");
	if (!f.Open())
	{
		Debug::Info("World[%s]: WDL: Error opening.", templateMap->Get_Directory_cstr());
		return;
	}

	// Offsets to MARE
	uint32_t MARE_Offsets[TILES_IN_MAP][TILES_IN_MAP];
	memset(MARE_Offsets, 0x00, TILES_IN_MAP * TILES_IN_MAP * sizeof(uint32_t));

	char fourcc[5];
	uint32_t size;

	while (!f.IsEof())
	{
		memset(fourcc, 0, 4);
		size = 0;
		f.ReadBytes(fourcc, 4);
		f.ReadBytes(&size, 4);
		flipcc(fourcc);
		fourcc[4] = 0;
		if (size == 0) continue;
		size_t nextpos = f.GetPos() + size;

		if (strcmp(fourcc, "MVER") == 0)
		{
			uint32_t version;
			f.ReadBytes(&version, 4);
			assert3(version == 18, "Version mismatch != 18", std::to_string(version).c_str());
		}
		else if (strncmp(fourcc, "MWMO", 4) == 0) // Filenames for WMO that appear in the low resolution map. Zero terminated strings.
		{
			char* buf = new char[size + 1];
			f.ReadBytes(buf, size);
			buf[size] = 0;
			char* p = buf;
			while (p < buf + size)
			{
				string name(p);
				p += strlen(p) + 1;

				m_Map_GlobalWMOs.AddLowResWMOName(name);
			}
			delete[] buf;
		}
		else if (strncmp(fourcc, "MWID", 4) == 0) // List of indexes into the MWMO chunk.
		{
		}
		else if (strncmp(fourcc, "MODF", 4) == 0) // Placement information for the WMO. Appears to be the same 64 byte structure used in the WDT and ADT MODF chunks.
		{
			uint32_t count = size / WMOPlacementInfo::__size;

			for (size_t i = 0; i < count; i++)
			{
				WMOPlacementInfo* placement = new WMOPlacementInfo;
				f.ReadBytes(placement, WMOPlacementInfo::__size);
				m_Map_GlobalWMOs.AddLowResWMOPlacementInfo(placement);
			}
		}
		else if (strncmp(fourcc, "MAOF", 4) == 0) // Contains 64*64 = 4096 unsigned 32-bit integers, these are absolute offsets in the file to each map m_TileExists's MapAreaLow-array-entry. For unused tiles the value is 0.
		{
			f.ReadBytes(MARE_Offsets, TILES_IN_MAP * TILES_IN_MAP * sizeof(uint32_t));
		}
		else if (strncmp(fourcc, "MARE", 4) == 0)
		{
			// Heightmap for one map m_TileExists. Contains 17*17 + 16*16 = 545 signed 16-bit integers. So a 17 by 17 grid of height values is given, with additional height values in between grid points. Here, the "outer" 17x17 points are listed (in the usual row major order), followed by 16x16 "inner" points. The height values are on the same scale as those used in the regular height maps.
		}
		else if (strncmp(fourcc, "MAHO", 4) == 0)
		{
			// After each MARE chunk there follows a MAHO (MapAreaHOles) chunk. It may be left out if the data is supposed to be 0 all the time. Its an array of 16 shorts. Each short is a bitmask. If the bit is not set, there is a hole at this position.
		}
		else
		{
			Debug::Info("Map[%s]: WDL: Chunks [%s], Size [%d] not implemented.", templateMap->Get_Directory_cstr(), fourcc, size);
		}
		f.Seek(nextpos);
	}

	// Buffer
	short tilebuf[17 * 17];
	short tilebuf2[16 * 16];

	// Minimap
	glGenTextures(1, &minimap);
	uint32_t* texbuf = new uint32_t[512 * 512];
	memset(texbuf, 0, 512 * 512 * 4);

	// Heightmap
	vec3 lowres[17][17];
	vec3 lowsub[16][16];

	for (int j = 0; j < 64; j++)
	{
		for (int i = 0; i < 64; i++)
		{
			if (MARE_Offsets[j][i])
			{
				// Read data
				f.Seek(MARE_Offsets[j][i] + 4 + 4);
				f.ReadBytes(tilebuf, 17 * 17 * 2);
				f.ReadBytes(tilebuf2, 16 * 16 * 2);

				// make minimap
				// for a 512x512 minimap texture, and 64x64 tiles, one m_TileExists is 8x8 pixels
				for (int z = 0; z < 8; z++)
				{
					for (int x = 0; x < 8; x++)
					{
						short hval = tilebuf[(z * 2) * 17 + x * 2]; // for now
						uint8_t r, g, b;
						if (hval < 0)
						{
							// water = blue
							if (hval < -511) hval = -511;
							hval /= -2;
							r = g = 0;
							b = 255 - hval;
						}
						else
						{
							// above water = should apply a palette :(
							/*
							float fh = hval / 1600.0f;
							if (fh > 1.0f) fh = 1.0f;
							uint8_t c = (uint8_t) (fh * 255.0f);
							r = g = b = c;
							*/

							// green: 20,149,7		0-600
							// brown: 137, 84, 21	600-1200
							// gray: 96, 96, 96		1200-1600
							// white: 255, 255, 255
							uint8_t r1, r2, g1, g2, b1, b2;
							float t;

							if (hval < 600)
							{
								r1 = 20;
								r2 = 137;
								g1 = 149;
								g2 = 84;
								b1 = 7;
								b2 = 21;
								t = hval / 600.0f;
							}
							else if (hval < 1200)
							{
								r2 = 96;
								r1 = 137;
								g2 = 96;
								g1 = 84;
								b2 = 96;
								b1 = 21;
								t = (hval - 600) / 600.0f;
							}
							else /*if (hval < 1600)*/
							{
								r1 = 96;
								r2 = 255;
								g1 = 96;
								g2 = 255;
								b1 = 96;
								b2 = 255;
								if (hval >= 1600) hval = 1599;
								t = (hval - 1200) / 600.0f;
							}

							// TODO: add a regular palette here

							r = (uint8_t)(r2*t + r1*(1.0f - t));
							g = (uint8_t)(g2*t + g1*(1.0f - t));
							b = (uint8_t)(b2*t + b1*(1.0f - t));
						}

						texbuf[(j * 8 + z) * 512 + i * 8 + x] = (r) | (g << 8) | (b << 16) | (255 << 24);
					}
				}

				for (int y = 0; y < 17; y++)
				{
					for (int x = 0; x < 17; x++)
					{
						lowres[y][x] = vec3(C_TileSize*(i + x / 16.0f), tilebuf[y * 17 + x], C_TileSize*(j + y / 16.0f));
					}
				}

				for (int y = 0; y < 16; y++)
				{
					for (int x = 0; x < 16; x++)
					{
						lowsub[y][x] = vec3(C_TileSize*(i + (x + 0.5f) / 16.0f), tilebuf2[y * 16 + x], C_TileSize*(j + (y + 0.5f) / 16.0f));
					}
				}

				GLuint dl;
				dl = glGenLists(1);
				glNewList(dl, GL_COMPILE);

				// draw tiles 17*17 + 16*16
				glBegin(GL_TRIANGLES);
				for (int y = 0; y < 16; y++)
				{
					for (int x = 0; x < 16; x++)
					{
						glVertex3fv(glm::value_ptr(lowres[y][x]));
						glVertex3fv(glm::value_ptr(lowsub[y][x]));
						glVertex3fv(glm::value_ptr(lowres[y][x + 1]));

						glVertex3fv(glm::value_ptr(lowres[y][x + 1]));
						glVertex3fv(glm::value_ptr(lowsub[y][x]));
						glVertex3fv(glm::value_ptr(lowres[y + 1][x + 1]));

						glVertex3fv(glm::value_ptr(lowres[y + 1][x + 1]));
						glVertex3fv(glm::value_ptr(lowsub[y][x]));
						glVertex3fv(glm::value_ptr(lowres[y + 1][x]));

						glVertex3fv(glm::value_ptr(lowres[y + 1][x]));
						glVertex3fv(glm::value_ptr(lowsub[y][x]));
						glVertex3fv(glm::value_ptr(lowres[y][x]));
					}
				}
				glEnd();
				glEndList();
				lowrestiles[j][i] = dl;
			}
		}
	}

	// Finish minimap
	glBindTexture(GL_TEXTURE_2D, minimap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, texbuf);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	delete[] texbuf;
}

void Map::Tick()
{
	bool loading = false;

	int midTile = static_cast<int>(RENDERED_TILES / 2);
	if (current[midTile][midTile] != nullptr || outOfBounds)
	{
		if (outOfBounds ||
			(_Camera->Position.x < current[midTile][midTile]->xbase) ||
			(_Camera->Position.x > (current[midTile][midTile]->xbase + C_TileSize)) ||
			(_Camera->Position.z < current[midTile][midTile]->zbase) ||
			(_Camera->Position.z > (current[midTile][midTile]->zbase + C_TileSize)))
		{

			enteredTileX = static_cast<int>(_Camera->Position.x / C_TileSize);
			enteredTileZ = static_cast<int>(_Camera->Position.z / C_TileSize);

			loading = true;
		}
	}

	if (loading)
	{
		if (enteredTileX != -1 && enteredTileZ != -1)
		{
			enterTile(enteredTileX, enteredTileZ);
			enteredTileX = enteredTileZ = -1;
			loading = false;
		}
	}
}

void Map::RenderSky()
{
	for (int i = 0; i < RENDERED_TILES; i++)
	{
		for (int j = 0; j < RENDERED_TILES; j++)
		{
			// OOB check
			if (IsBadTileIndex(i, j))
			{
				continue;
			}

			if (current[i][j] != nullptr)
			{
				current[i][j]->drawSky();
			}

			if (_World->hadSky)
			{
				break;
			}
		}
	}
}

void Map::RenderLowResTiles()
{
	const int lrr = 3;
	for (int i = currentTileX - lrr; i <= currentTileX + lrr; i++)
	{
		for (int j = currentTileZ - lrr; j <= currentTileZ + lrr; j++)
		{
			// OOB check
			if (IsBadTileIndex(i, j))
			{
				continue;
			}

			// Don't draw current tile
			if (i == currentTileX && j == currentTileZ)
			{
				continue;
			}

			// TODO: some annoying visual artifacts when the verylowres terrain overlaps
			// maptiles that are close (1-off) - figure out how to fix.
			// still less annoying than hoels in the horizon when only 2-off verylowres tiles are drawn
			if (lowrestiles[j][i])
			{
				glCallList(lowrestiles[j][i]);
			}
		}
	}
}

void Map::RenderTiles()
{
	for (int i = 0; i < RENDERED_TILES; i++)
		for (int j = 0; j < RENDERED_TILES; j++)
			if (current[i][j] != nullptr)
				current[i][j]->draw();
}

void Map::RenderObjects()
{
	for (int i = 0; i < RENDERED_TILES; i++)
		for (int j = 0; j < RENDERED_TILES; j++)
			if (current[i][j] != nullptr)
				current[i][j]->drawObjects();
}

void Map::RenderModels()
{
	for (int i = 0; i < RENDERED_TILES; i++)
		for (int j = 0; j < RENDERED_TILES; j++)
			if (current[i][j] != nullptr)
				current[i][j]->drawModels();
}

void Map::RenderWater()
{
	for (int i = 0; i < RENDERED_TILES; i++)
		for (int j = 0; j < RENDERED_TILES; j++)
			if (current[i][j] != nullptr)
				current[i][j]->drawWater();
}

void Map::enterTile(int x, int z)
{
	if (IsBadTileIndex(x, z))
	{
		outOfBounds = true;
		return;
	}
	else
	{
		outOfBounds = !m_TileExists[x][z];
	}

	currentTileX = x;
	currentTileZ = z;

	for (int i = 0; i < RENDERED_TILES; i++)
	{
		for (int j = 0; j < RENDERED_TILES; j++)
		{
			current[i][j] = LoadTile(x - static_cast<int>(RENDERED_TILES / 2) + i, z - static_cast<int>(RENDERED_TILES / 2) + j);
		}
	}
}

MapTile* Map::LoadTile(int x, int z)
{
	if (IsBadTileIndex(x, z))
	{
		return nullptr;
	}

	if (!m_TileExists[x][z])
	{
		return nullptr;
	}

	// Try get m_TileExists from cache
	int firstnull = MAPTILECACHESIZE;
	for (int i = 0; i < MAPTILECACHESIZE; i++)
	{
		if ((maptilecache[i] != 0) && (maptilecache[i]->indexX == x) && (maptilecache[i]->indexZ == z))
		{
			return maptilecache[i];
		}

		if (maptilecache[i] == nullptr && i < firstnull)
		{
			firstnull = i;
		}
	}

	// ok we need to find a place in the cache
	if (firstnull == MAPTILECACHESIZE)
	{
		int score, maxscore = 0, maxidx = 0;
		// oh shit we need to throw away a m_TileExists
		for (int i = 0; i < MAPTILECACHESIZE; i++)
		{
			if (maptilecache[i] == 0)
				continue;
			score = abs(maptilecache[i]->indexX - currentTileX) + abs(maptilecache[i]->indexZ - currentTileZ);
			if (score > maxscore)
			{
				maxscore = score;
				maxidx = i;
			}
		}

		// maxidx is the winner (loser)
		delete maptilecache[maxidx];
		maptilecache[maxidx] = 0;
		firstnull = maxidx;
	}

	maptilecache[firstnull] = new MapTile(x, z, templateMap->Get_Directory_cstr());
	return maptilecache[firstnull];
}

uint32_t Map::getAreaID()
{
	MapTile* curTile;

	int tileX, tileZ, chunkX, chunkZ;
	tileX = (int)(_Camera->Position.x / C_TileSize);
	tileZ = (int)(_Camera->Position.z / C_TileSize);

	chunkX = (int)(fmod(_Camera->Position.x, C_TileSize) / C_ChunkSize);
	chunkZ = (int)(fmod(_Camera->Position.z, C_TileSize) / C_ChunkSize);

	if (
		(tileX < currentTileX - static_cast<int>(RENDERED_TILES / 2)) || (tileX > currentTileX + static_cast<int>(RENDERED_TILES / 2)) ||
		(tileZ < currentTileZ - static_cast<int>(RENDERED_TILES / 2)) || (tileZ > currentTileZ + static_cast<int>(RENDERED_TILES / 2))
		)
		return 0;

	curTile = current[tileZ - currentTileZ + static_cast<int>(RENDERED_TILES / 2)][tileX - currentTileX + static_cast<int>(RENDERED_TILES / 2)];
	if (curTile == 0)
		return 0;

	MapChunk* curChunk = curTile->getChunk(chunkX, chunkZ);

	if (curChunk == nullptr)
		return 0;

	return curChunk->areaID;
}