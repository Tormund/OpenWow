#include "stdafx.h"

// General
#include "Map.h"

Map::Map()
{
	// Load default variables
	mapHasTerrain = false;
	tilesCount = 0;
	memset(m_TileExists, 0, sizeof(m_TileExists));
	memset(m_TileIsWater, 0, sizeof(m_TileIsWater));
	memset(lowrestiles, 0, sizeof(lowrestiles));
	minimap = 0;
	memset(maptilecache, 0, sizeof(maptilecache));
	currentTileX = currentTileZ = -1;
	memset(current, 0, sizeof(current));
	outOfBounds = false;

	CreateMapArrays();

	ADDCONSOLECOMMAND_CLASS("map_clear", Map, ClearCache);
}

Map::~Map()
{
	if (mapstrip)
	{
		delete[] mapstrip;
	}

	if (mapstrip2)
	{
		delete[] mapstrip2;
	}
}

//

void Map::CreateMapArrays()
{
	// default strip indices
	short *defstrip = new short[stripsize];
	for (int i = 0; i < stripsize; i++)
		defstrip[i] = i; // note: this is ugly and should be handled in stripify

	mapstrip = new short[stripsize];
	stripify<short>(defstrip, mapstrip);
	delete[] defstrip;

	//

	defstrip = new short[stripsize2];
	for (int i = 0; i < stripsize2; i++)
		defstrip[i] = i; // note: this is ugly and should be handled in stripify
	mapstrip2 = new short[stripsize2];
	stripify2<short>(defstrip, mapstrip2);
	delete[] defstrip;

	//

	vec2* vt;
	float tx, ty;

	// init texture coordinates for detail map:
	vt = dataDetail;
	const float detail_half = 0.5f * detail_size / 8.0f;
	for (int j = 0; j < 17; j++)
	{
		for (int i = 0; i < ((j % 2) ? 8 : 9); i++)
		{
			tx = detail_size / 8.0f * i;
			ty = detail_size / 8.0f * j * 0.5f;
			if (j % 2)
			{
				// offset by half
				tx += detail_half;
			}
			*vt++ = vec2(tx, ty);
		}
	}

	//

	// init texture coordinates for alpha map:
	vt = dataAlpha;
	const float alpha_half = 0.5f * 1.0f / 8.0f;
	for (int j = 0; j < 17; j++)
	{
		for (int i = 0; i < ((j % 2) ? 8 : 9); i++)
		{
			tx = 1.0f / 8.0f * i;
			ty = 1.0f / 8.0f * j * 0.5f;
			if (j % 2)
			{
				// offset by half
				tx += alpha_half;
			}
			//*vt++ = vec2(tx*0.95f, ty*0.95f);
			const int divs = 32;
			const float inv = 1.0f / divs;
			const float mul = (divs - 1.0f);
			*vt++ = vec2(tx*(mul*inv), ty*(mul*inv));
		}
	}
}

void Map::InitGlobalsWMOs()
{
	// Load global WMO
	Debug::Info("Map_GlobalWMOs[]: Global WMO exists [%s].", globalWMOExists ? "true" : "false");
	if (globalWMOExists)
	{
		WMO* wmo = _WMOsMgr->Add(globalWMOName);
		globalWMO = new WMOInstance(wmo, globalWMOplacementInfo);
	}

	// Load low-resolution WMOs
	Debug::Info("Map_GlobalWMOs[]: Low WMOs count [%d].", lowResolutionWMOsCount);
	for (uint32_t i = 0; i < lowResolutionWMOsCount; i++)
	{
		const string name = lowResolutionWMOsNames[lowResolutionWMOsplacementInfo[i]->nameIndex];

		WMO* wmo = _WMOsMgr->Add(name);
		WMOInstance* inst = new WMOInstance(wmo, lowResolutionWMOsplacementInfo[i]);
		lowResolutionWMOs.push_back(inst);
	}
}

//

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
				Map_Disable_Something = 0x8,			// Disables something. No idea what. Another rendering thing. Someone may check all them in wild life..
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
		else if (strcmp(fourcc, "MAIN") == 0) // Map tile table. Contains 64x64 = 4096 records of 8 bytes each.
		{
			enum MAIN_Flags
			{
				Map_HasADT = 0x01,
				Map_AllWater = 0x02,
				Map_Loaded = 0x04,
			};

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

				globalWMOName = string(buf);

				delete[] buf;
			}
		}
		else if (strcmp(fourcc, "MODF") == 0)
		{
			uint32_t globalWMOCount = WMOPlacementInfo::__size;
			assert4(globalWMOCount > 1, "Map has more then 1 global WMO ", templateMap->Get_Directory_cstr(), std::to_string(globalWMOCount).c_str());

			if (globalWMOCount > 0)
			{
				globalWMOplacementInfo = new WMOPlacementInfo;
				f.ReadBytes(globalWMOplacementInfo, WMOPlacementInfo::__size);

				globalWMOExists = true;
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
		Debug::Info("Map[%s]: Is tile-based map.", templateMap->Get_Directory_cstr());
	}

	LoadLowTerrain();
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
	uint32_t MARE_Offsets[C_TilesInMap][C_TilesInMap];
	memset(MARE_Offsets, 0x00, C_TilesInMap * C_TilesInMap * sizeof(uint32_t));

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
			WOWCHUNK_READ_STRINGS_BEGIN

#ifdef WMO_INCL
			lowResolutionWMOsNames.push_back(_string);
#endif
			WOWCHUNK_READ_STRINGS_END;
		}
		else if (strncmp(fourcc, "MWID", 4) == 0) // List of indexes into the MWMO chunk.
		{
		}
		else if (strncmp(fourcc, "MODF", 4) == 0) // Placement information for the WMO. Appears to be the same 64 byte structure used in the WDT and ADT MODF chunks.
		{
			uint32_t count = size / WMOPlacementInfo::__size;

#ifdef WMO_INCL
			for (uint32_t i = 0; i < count; i++)
			{
				WMOPlacementInfo* placement = new WMOPlacementInfo;
				f.ReadBytes(placement, WMOPlacementInfo::__size);
				lowResolutionWMOsplacementInfo.push_back(placement);
			}
#endif
		}
		else if (strncmp(fourcc, "MAOF", 4) == 0) // Contains 64*64 = 4096 unsigned 32-bit integers, these are absolute offsets in the file to each map tile's MapAreaLow-array-entry. For unused tiles the value is 0.
		{
			f.ReadBytes(MARE_Offsets, C_TilesInMap * C_TilesInMap * sizeof(uint32_t));
		}
		else if (strncmp(fourcc, "MARE", 4) == 0) // Heightmap for one map tile.
		{
			 // Contains 17*17 + 16*16 = 545 signed 16-bit integers. So a 17 by 17 grid of height values is given, with additional height values in between grid points. Here, the "outer" 17x17 points are listed (in the usual row major order), followed by 16x16 "inner" points. The height values are on the same scale as those used in the regular height maps.
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
				// for a 512x512 minimap texture, and 64x64 tiles, one tile is 8x8 pixels
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


				vector<vec3> vecrtices;

				for (int y = 0; y < 16; y++)
				{
					for (int x = 0; x < 16; x++)
					{
						vecrtices.push_back(lowres[y][x]);
						vecrtices.push_back(lowsub[y][x]);
						vecrtices.push_back(lowres[y][x + 1]);

						vecrtices.push_back(lowres[y][x + 1]);
						vecrtices.push_back(lowsub[y][x]);
						vecrtices.push_back(lowres[y + 1][x + 1]);

						vecrtices.push_back(lowres[y + 1][x + 1]);
						vecrtices.push_back(lowsub[y][x]);
						vecrtices.push_back(lowres[y + 1][x]);

						vecrtices.push_back(lowres[y + 1][x]);
						vecrtices.push_back(lowsub[y][x]);
						vecrtices.push_back(lowres[y][x]);
					}
				}

				glGenBuffers(1, &lowrestiles[j][i]);
				glBindBuffer(GL_ARRAY_BUFFER, lowrestiles[j][i]);

				glBufferData(GL_ARRAY_BUFFER, 16*16*12 * 12, &vecrtices[0], GL_STATIC_DRAW);

				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
		}
	}

	// Finish minimap
	glBindTexture(GL_TEXTURE_2D, minimap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, texbuf);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	delete[] texbuf;


	// Init global WMOs
#ifdef WMO_INCL
	InitGlobalsWMOs();
#endif
}

void Map::UnloadMap()
{
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			if (lowrestiles[i][j] != 0)
			{
				glDeleteBuffers(1, &lowrestiles[i][j]);
			}
		}
	}

	for (int i = 0; i < C_TilesCacheSize; i++)
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

//

void Map::Tick()
{
	bool loading = false;
	int enteredTileX, enteredTileZ;
	int midTile = static_cast<uint32_t>(C_RenderedTiles / 2);
	if (current[midTile][midTile] != nullptr || outOfBounds)
	{
		if (outOfBounds ||
			(_Camera->Position.x < current[midTile][midTile]->m_GamePositionX) ||
			(_Camera->Position.x > (current[midTile][midTile]->m_GamePositionX + C_TileSize)) ||
			(_Camera->Position.z < current[midTile][midTile]->m_GamePositionZ) ||
			(_Camera->Position.z > (current[midTile][midTile]->m_GamePositionZ + C_TileSize)))
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

//

void Map::RenderSky()
{
	for (int i = 0; i < C_RenderedTiles; i++)
	{
		for (int j = 0; j < C_RenderedTiles; j++)
		{
			if (current[i][j] != nullptr)
			{
				current[i][j]->drawSky();
			}

			if (_EnvironmentManager->m_HasSky)
			{
				break;
			}
		}
	}
}

void Map::RenderLowResTiles()
{

	for (int i = 0; i < C_TilesInMap; i++)
		for (int j = 0; j < C_TilesInMap; j++)
			if (lowrestiles[i][j])
			{
				glBindBuffer(GL_ARRAY_BUFFER, lowrestiles[i][j]);

				// Vertex
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

				glDrawArrays(GL_TRIANGLES, 0, 16 * 16 * 12);
				PERF_INC(PERF_MAP_LOWRESOLUTION);

				glDisableVertexAttribArray(0);

				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}



	/*const int lrr = 5;
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
			if (lowrestiles[i][j])
			{
				glBindBuffer(GL_ARRAY_BUFFER, lowrestiles[i][j]);

				// Vertex
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

				//glDrawElements(GL_TRIANGLE_STRIP, striplen, GL_UNSIGNED_SHORT, strip);
				glDrawBuffer(GL_TRIANGLES);
				PERF_INC(PERF_MAP_LOWRES_TILES);

				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
		}
	}*/
}

void Map::RenderTiles()
{
	for (int i = 0; i < C_TilesCacheSize; i++)
	{
		if (maptilecache[i] != nullptr)
		{
			maptilecache[i]->draw();
		}
	}

	/*for (int i = 0; i < C_RenderedTiles; i++)
		for (int j = 0; j < C_RenderedTiles; j++)
			if (current[i][j] != nullptr)
				current[i][j]->draw();*/
}

void Map::RenderObjects()
{
	for (int i = 0; i < C_RenderedTiles; i++)
		for (int j = 0; j < C_RenderedTiles; j++)
			if (current[i][j] != nullptr)
				current[i][j]->drawObjects();
}

void Map::RenderModels()
{
	for (int i = 0; i < C_RenderedTiles; i++)
		for (int j = 0; j < C_RenderedTiles; j++)
			if (current[i][j] != nullptr)
				current[i][j]->drawModels();
}

void Map::RenderWater()
{
	for (int i = 0; i < C_RenderedTiles; i++)
		for (int j = 0; j < C_RenderedTiles; j++)
			if (current[i][j] != nullptr)
				current[i][j]->drawWater();
}

//

void Map::enterTile(int x, int z)
{
	if (IsBadTileIndex(x, z))
	{
		outOfBounds = true;
		return;
	}

	outOfBounds = !m_TileExists[x][z];

	currentTileX = x;
	currentTileZ = z;

	for (int i = 0; i < C_RenderedTiles; i++)
	{
		for (int j = 0; j < C_RenderedTiles; j++)
		{
			current[i][j] = LoadTile(x - static_cast<int>(C_RenderedTiles / 2) + i, z - static_cast<int>(C_RenderedTiles / 2) + j);
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

	// Try get tile from cache
	int firstnull = C_TilesCacheSize;
	for (int i = 0; i < C_TilesCacheSize; i++)
	{
		if ((maptilecache[i] != nullptr) && (maptilecache[i]->m_IndexX == x) && (maptilecache[i]->m_IndexZ == z))
		{
			return maptilecache[i];
		}

		if (maptilecache[i] == nullptr && i < firstnull)
		{
			firstnull = i;
		}
	}

	// ok we need to find a place in the cache
	if (firstnull == C_TilesCacheSize)
	{
		int score, maxscore = 0, maxidx = 0;
		// oh shit we need to throw away a tile
		for (int i = 0; i < C_TilesCacheSize; i++)
		{
			if (maptilecache[i] == nullptr)
			{
				continue;
			}

			score = abs(maptilecache[i]->m_IndexX - currentTileX) + abs(maptilecache[i]->m_IndexZ - currentTileZ);

			if (score > maxscore)
			{
				maxscore = score;
				maxidx = i;
			}
		}

		// maxidx is the winner (loser)
		delete maptilecache[maxidx];
		maptilecache[maxidx] = nullptr;
		firstnull = maxidx;
	}

	// Create new tile
	maptilecache[firstnull] = new MapTile(x, z);
	if (!maptilecache[firstnull]->Init(templateMap->Get_Directory_cstr()))
	{
		delete maptilecache[firstnull];
		Debug::Info("Map[%d]: Error loading tile [%d, %d].", GetPath().c_str(), x, z);
		return nullptr;
	}

	return maptilecache[firstnull];
}

void Map::ClearCache()
{
	for (int i = 0; i < C_TilesCacheSize; i++)
	{
		if (maptilecache[i] != nullptr && !IsTileInCurrent(maptilecache[i]))
		{
			delete maptilecache[i];
			maptilecache[i] = nullptr;
		}
	}
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
		(tileX < currentTileX - static_cast<int>(C_RenderedTiles / 2)) || (tileX > currentTileX + static_cast<int>(C_RenderedTiles / 2)) ||
		(tileZ < currentTileZ - static_cast<int>(C_RenderedTiles / 2)) || (tileZ > currentTileZ + static_cast<int>(C_RenderedTiles / 2))
		)
		return 0;

	curTile = current[tileZ - currentTileZ + static_cast<int>(C_RenderedTiles / 2)][tileX - currentTileX + static_cast<int>(C_RenderedTiles / 2)];
	if (curTile == 0)
		return 0;

	MapChunk* curChunk = curTile->getChunk(chunkX, chunkZ);

	if (curChunk == nullptr)
		return 0;

	return curChunk->areaID;
}

bool Map::IsTileInCurrent(MapTile * _mapTile)
{
	for (int i = 0; i < C_RenderedTiles; i++)
		for (int j = 0; j < C_RenderedTiles; j++)
			if (current[i][j] != nullptr)
				if (current[i][j] == _mapTile)
					return true;
	return false;
}
