#include "../stdafx.h"

// General
#include "Map_Tile.h"

// Additional
#include "../Environment/EnvironmentManager.h"
#include "../MDX/ModelsManager.h"
#include "../WMO/WMO_Manager.h"

struct MapTileHeader
{
	uint32 flags;

	uint32 MCIN;
	uint32 MTEX;
	uint32 MMDX;
	uint32 MMID;
	uint32 MWMO;
	uint32 MWID;
	uint32 MDDF;
	uint32 MODF;
	uint32 MFBO;
	uint32 MH2O;
	uint32 MTXF;

	uint8 mamp_value;             // Cata+, explicit MAMP chunk overrides data
	uint8 padding[3];
	uint32 unused[3];
};

MapTile::MapTile(int x0, int z0) : m_IndexX(x0), m_IndexZ(z0)
{

#ifdef WMO_INCL
	wmoCount = 0;
#endif

#ifdef MDX_INCL
	mdxCount = 0;
#endif

	m_GamePositionX = x0 * C_TileSize;
	m_GamePositionZ = z0 * C_TileSize;

	for (uint32 i = 0; i < C_ChunksInTile; i++)
	{
		for (uint32 j = 0; j < C_ChunksInTile; j++)
		{
			chunks[i][j] = nullptr;
		}
	}
}

MapTile::~MapTile()
{
	Modules::log().Info("MapTile[%d, %d]: Unloading tile...", m_IndexX, m_IndexZ);

	for (uint32 i = 0; i < C_ChunksInTile; i++)
	{
		for (uint32 j = 0; j < C_ChunksInTile; j++)
		{
			if (chunks[i][j] != nullptr)
			{
				delete chunks[i][j];
				chunks[i][j] = nullptr;
			}
		}
	}

	for (auto it = m_DiffuseTextures.begin(); it != m_DiffuseTextures.end(); ++it)
	{
		_TexturesMgr->Delete(*it);
	}

	for (auto it = m_SpecularTextures.begin(); it != m_SpecularTextures.end(); ++it)
	{
		_TexturesMgr->Delete(*it);
	}

#ifdef WMO_INCL
	for (auto it = wmoNames.begin(); it != wmoNames.end(); ++it)
	{
		_WMOsMgr->Delete(*it);
	}
	ERASE_VECTOR(wmoInstances);
#endif

#ifdef MDX_INCL
	for (auto it = mdxNames.begin(); it != mdxNames.end(); ++it)
	{
		_ModelsMgr->Delete(*it);
	}
	ERASE_VECTOR(mdxInstances);
#endif

	Modules::log().Green("MapTile[%d, %d]: Unloaded.", m_IndexX, m_IndexZ);
}

bool MapTile::Init(cstring _filename)
{
	for (uint32 i = 0; i < C_ChunksInTile; i++)
	{
		for (uint32 j = 0; j < C_ChunksInTile; j++)
		{
			chunks[i][j] = new MapChunk(this);
		}
	}

	Modules::log().Info("MapTile[%d, %d, %s]: Loading...", m_IndexX, m_IndexZ, _filename.c_str());

	for (int fileindex = 0; fileindex < 3; fileindex++)
	{
		load_phases phase;
		switch (fileindex)
		{
			case 0:
			phase = main_file;
			break;

			case 1:
			phase = tex;
			break;

			case 2:
			phase = obj;
			break;
		}

		if (!parse_adt(_filename, phase))
		{
			return false;
		}
	}

	Modules::log().Green("MapTile[%d, %d, %s]: Loaded!", m_IndexX, m_IndexZ, _filename.c_str());

	// init quadtree
	//topnode = new MapNode(x, z, 16);
	//topnode->setup(this);

	// Verteces
	//technique = new LightTechnique();
	//technique->Init();

	return true;
}

bool MapTile::parse_adt(cstring _filename, load_phases _phase)
{
	char name[256];
	switch (_phase)
	{
		case main_file:
		sprintf_s(name, "World\\Maps\\%s\\%s_%d_%d.adt", _filename.c_str(), _filename.c_str(), m_IndexX, m_IndexZ);
		break;

		case tex:
		sprintf_s(name, "World\\Maps\\%s\\%s_%d_%d_tex%d.adt", _filename.c_str(), _filename.c_str(), m_IndexX, m_IndexZ, 0 /*1*/);
		break;

		case obj:
		sprintf_s(name, "World\\Maps\\%s\\%s_%d_%d_obj0.adt", _filename.c_str(), _filename.c_str(), m_IndexX, m_IndexZ);
		break;

		default:
		assert1(false);
		break;
	}

	File f = name;
	if (!f.Open())
	{
		Modules::log().Error("MapTile[%d, %d, %s]: Error open file!", m_IndexX, m_IndexZ, name);
		return false;
	}


	int chunkI = 0;
	int chunkJ = 0;

	char fourcc[5];
	uint32 size;
	while (!f.IsEof())
	{
		memset(fourcc, 0, 4);
		f.ReadBytes(fourcc, 4);
		f.ReadBytes(&size, 4);
		flipcc(fourcc);
		fourcc[4] = 0;
		if (size == 0) continue;
		size_t nextpos = f.GetPos() + size;

		if (strncmp(fourcc, "MVER", 4) == 0)
		{
			uint32 version;
			f.ReadBytes(&version, 4);
			assert4(version == 18, "Version mismatch != 18", std::to_string(version).c_str(), name);
		}
		else if (strncmp(fourcc, "MHDR", 4) == 0) // Contains offsets relative to &MHDR.data in the file for specific chunks.
		{
		}
		else if (strncmp(fourcc, "MTEX", 4) == 0) // List of textures used for texturing the terrain in this map m_TileExists.
		{
			WOWCHUNK_READ_STRINGS_BEGIN

				// Preload diffuse texture
				m_DiffuseTextures.push_back(_TexturesMgr->Add(_string));

			// Preload specular texture
			string specularTextureName = _string;
			specularTextureName.insert(specularTextureName.length() - 4, "_s");
			m_SpecularTextures.push_back(_TexturesMgr->Add(specularTextureName));

			WOWCHUNK_READ_STRINGS_END
		}
		else if (strncmp(fourcc, "MMDX", 4) == 0) // List of filenames for M2 models that appear in this map m_TileExists.
		{
			WOWCHUNK_READ_STRINGS_BEGIN
#ifdef MDX_INCL
				_ModelsMgr->Add(_string);
			mdxNames.push_back(_string);
#endif
			WOWCHUNK_READ_STRINGS_END
		}
		else if (strncmp(fourcc, "MMID", 4) == 0) // List of offsets of model filenames in the MMDX chunk.
		{
		}
		else if (strncmp(fourcc, "MWMO", 4) == 0) // List of filenames for WMOs (world map objects) that appear in this map m_TileExists.
		{
			WOWCHUNK_READ_STRINGS_BEGIN
#ifdef WMO_INCL
				_WMOsMgr->Add(_string);
			wmoNames.push_back(_string);
#endif
			WOWCHUNK_READ_STRINGS_END
		}
		else if (strncmp(fourcc, "MWID", 4) == 0) // List of offsets of WMO filenames in the MWMO chunk.
		{
		}
		else if (strncmp(fourcc, "MDDF", 4) == 0) // Placement information for doodads (M2 models).
		{
#ifdef MDX_INCL
			mdxCount = size / ModelPlacementInfo::__size;
			for (uint32 i = 0; i < mdxCount; i++)
			{
				ModelInstance* inst = new ModelInstance(f);

				MDX *model = (MDX*)_ModelsMgr->objects[mdxNames[inst->placementInfo->nameId]];
				assert1(model != nullptr);
				inst->SetModel(model);

				mdxInstances.push_back(inst);
			}
#endif
		}
		else if (strncmp(fourcc, "MODF", 4) == 0) // Placement information for WMOs.
		{
#ifdef WMO_INCL
			wmoCount = size / WMOPlacementInfo::__size;
			for (uint32 i = 0; i < wmoCount; i++)
			{
				uint32 wmoIndex;
				f.ReadBytes(&wmoIndex, 4);
				f.SeekRelative(-4);

				WMO* wmo = (WMO*)_WMOsMgr->GetItemByName(wmoNames[wmoIndex]);
				WMOInstance* inst = new WMOInstance(wmo, f);
				wmoInstances.push_back(inst);
			}
#endif
		}
		else if (strncmp(fourcc, "MH2O", 4) == 0)
		{
			uint8* abuf = f.GetDataFromCurrent();

			for (uint32 i = 0; i < C_ChunksInTile * C_ChunksInTile; i++)
			{
				MH2O_Header* mh2o_Header = (MH2O_Header*)abuf;

				chunks[i / C_ChunksInTile][i % C_ChunksInTile]->CreateMH2OLiquid(f, mh2o_Header);

				abuf += sizeof(MH2O_Header);
			}
		}
		else if (strncmp(fourcc, "MCNK", 4) == 0)
		{
			chunks[chunkI][chunkJ]->init(f, _phase);
			chunkJ++;
			if (chunkJ == 16)
			{
				chunkJ = 0;
				chunkI++;
			}
		}
		else if (strncmp(fourcc, "MFBO", 4) == 0)
		{
			// A bounding box for flying.
			/*
			struct plane {
			short[3][3] height;
			};
			struct {
			plane maximum;
			plane minimum;
			} MFBO;
			*/
		}
		else if (strncmp(fourcc, "MTXF", 4) == 0)
		{
			/*
			This chunk is an array of integers that are 1 or 0. 1 means that the texture at the same position in the MTEX array has to be handled differentely. The size of this chunk is always the same as there are entries in the MTEX chunk.
			Simple as it is:
			struct MTFX
			{
			uint32 mode[nMTEX];
			}
			The textures with this extended rendering mode are no normal ones, but skyboxes. These skyboxes are getting added as a reflection layer on the terrain. This is used for icecubes reflecting clouds etc. The layer being the reflection one needs to have the 0x400 flag in the MCLY chunk.
			*/
		}
		else if (strncmp(fourcc, "MAMP", 4) == 0)
		{
		}
		else
		{
			Modules::log().Info("MapTile[%s]: No implement chunk %s [%d].", name, fourcc, size);
		}

		f.Seek(nextpos);
	}

	return true;
}

//

void MapTile::draw()
{
	for (uint32 i = 0; i < C_ChunksInTile; i++)
		for (uint32 j = 0; j < C_ChunksInTile; j++)
			if (chunks[i][j] != nullptr)
				chunks[i][j]->Render();
}

void MapTile::drawWater()
{
	for (uint32 i = 0; i < C_ChunksInTile; i++)
		for (uint32 j = 0; j < C_ChunksInTile; j++)
			if (chunks[i][j] != nullptr)
				if(chunks[i][j]->m_Liquid != nullptr)
					chunks[i][j]->m_Liquid->draw();
}

void MapTile::drawObjects()
{
#ifdef WMO_INCL
	for (auto it = wmoInstances.begin(); it != wmoInstances.end(); ++it)
	{
		(*it)->draw();
	}
#endif
}

void MapTile::drawSky()
{
#ifdef WMO_INCL
#ifdef MDX_INCL
	for (auto it = wmoInstances.begin(); it != wmoInstances.end(); ++it)
	{
		(*it)->GetWMO()->drawSkybox();

		if (_EnvironmentManager->m_HasSky)
		{
			break;
		}
	}
#endif
#endif
}

void MapTile::drawModels()
{
#ifdef MDX_INCL
	for (auto it = mdxInstances.begin(); it != mdxInstances.end(); ++it)
	{
		(*it)->draw();
	}
#endif
}

//



MapChunk* MapTile::getChunk(uint32 x, uint32 z)
{
	assert1(x < C_ChunksInTile && z < C_ChunksInTile);
	return chunks[x][z];
}
