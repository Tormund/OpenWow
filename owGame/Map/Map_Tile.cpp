#include "../stdafx.h"

// General
#include "Map_Tile.h"

// Additional
#include "../Environment/EnvironmentManager.h"
#include "../MDX/ModelsManager.h"
#include "../WMO/WMO_Manager.h"

struct ADT_MHDR
{
	struct
	{
		uint32 HAS_MFBO : 1;
		uint32 IS_NORTREND : 1;
		uint32 : 30;
	} flags;

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
	uint8 unk[15];
};

MapTile::MapTile(uint32 _intexX, uint32 _intexZ) : m_IndexX(_intexX), m_IndexZ(_intexZ)
{
	m_GamePositionX = _intexX * C_TileSize;
	m_GamePositionZ = _intexZ * C_TileSize;

	for (uint32 i = 0; i < C_ChunksInTile; i++)
	{
		for (uint32 j = 0; j < C_ChunksInTile; j++)
		{
			m_Chunks[i][j] = new MapChunk(this);
		}
	}
}

MapTile::~MapTile()
{
	Modules::log().Info("MapTile[%d, %d]: Unloading tile...", m_IndexX, m_IndexZ);

	//---------------------------------------------------------------------------------

	for (uint32 i = 0; i < C_ChunksInTile; i++)
	{
		for (uint32 j = 0; j < C_ChunksInTile; j++)
		{
			if (m_Chunks[i][j] != nullptr)
			{
				delete m_Chunks[i][j];
				m_Chunks[i][j] = nullptr;
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
	for (auto it = m_WMOsNames.begin(); it != m_WMOsNames.end(); ++it)
	{
		_WMOsMgr->Delete(*it);
	}
	ERASE_VECTOR(m_WMOsInstances);
#endif

#ifdef MDX_INCL
	for (auto it = m_MDXsNames.begin(); it != m_MDXsNames.end(); ++it)
	{
		_ModelsMgr->Delete(*it);
	}
	ERASE_VECTOR(m_MDXsInstances);
#endif

	//---------------------------------------------------------------------------------

	Modules::log().Green("MapTile[%d, %d]: Unloaded.", m_IndexX, m_IndexZ);
}

bool MapTile::Load(cstring _filename)
{
	//Modules::log().Info("MapTile[%d, %d, %s]: Loading...", m_IndexX, m_IndexZ, _filename.c_str());

	//---------------------------------------------------------------------------------

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

		if (!Load_SplitFile(_filename, phase))
		{
			return false;
		}
	}

	//---------------------------------------------------------------------------------

	Modules::log().Green("MapTile[%d, %d, %s]: Loaded!", m_IndexX, m_IndexZ, _filename.c_str());

	return true;
}

bool MapTile::Load_SplitFile(cstring _filename, load_phases _phase)
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
			assert1(version == 18);
		}
		else if (strncmp(fourcc, "MHDR", 4) == 0) // Contains offsets relative to &MHDR.data in the file for specific chunks.
		{
			ADT_MHDR header;
			f.ReadBytes(&header, sizeof(ADT_MHDR));
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
#ifdef MDX_INCL
			WOWCHUNK_READ_STRINGS_BEGIN
			//_ModelsMgr->Add(_string);
			//m_MDXsNames.push_back(_string);
			WOWCHUNK_READ_STRINGS_END
#endif
		}
		else if (strncmp(fourcc, "MMID", 4) == 0) // List of offsets of model filenames in the MMDX chunk.
		{
			// Currently unused
		}
		else if (strncmp(fourcc, "MWMO", 4) == 0) // List of filenames for WMOs (world map objects) that appear in this map m_TileExists.
		{
#ifdef WMO_INCL
			WOWCHUNK_READ_STRINGS_BEGIN
			//	_WMOsMgr->Add(_string);
			//m_WMOsNames.push_back(_string);
			WOWCHUNK_READ_STRINGS_END
#endif
		}
		else if (strncmp(fourcc, "MWID", 4) == 0) // List of offsets of WMO filenames in the MWMO chunk.
		{
			// Currently unused
		}
		else if (strncmp(fourcc, "MDDF", 4) == 0) // Placement information for doodads (M2 models).
		{
#ifdef MDX_INCL
			/*for (uint32 i = 0; i < size / ModelPlacementInfo::__size; i++)
			{
				ModelInstance* inst = new ModelInstance(f);

				MDX *model = (MDX*)_ModelsMgr->objects[m_MDXsNames[inst->placementInfo->nameId]];
				assert1(model != nullptr);
				inst->SetModel(model);

				m_MDXsInstances.push_back(inst);
			}*/
#endif
		}
		else if (strncmp(fourcc, "MODF", 4) == 0) // Placement information for WMOs.
		{
#ifdef WMO_INCL
			/*for (uint32 i = 0; i < size / WMOPlacementInfo::__size; i++)
			{
				uint32 wmoIndex;
				f.ReadBytes(&wmoIndex, 4);
				f.SeekRelative(-4);

				WMO* wmo = (WMO*)_WMOsMgr->GetItemByName(m_WMOsNames[wmoIndex]);
				WMOInstance* inst = new WMOInstance(wmo, f);
				m_WMOsInstances.push_back(inst);
			}*/
#endif
		}
		else if (strncmp(fourcc, "MH2O", 4) == 0) // Water
		{
			uint8* abuf = f.GetDataFromCurrent();

			for (uint32 i = 0; i < C_ChunksInTile * C_ChunksInTile; i++)
			{
				MH2O_Header* mh2o_Header = (MH2O_Header*)abuf;

				m_Chunks[i / C_ChunksInTile][i % C_ChunksInTile]->CreateMH2OLiquid(f, mh2o_Header);

				abuf += sizeof(MH2O_Header);
			}
		}
		else if (strncmp(fourcc, "MCNK", 4) == 0)
		{
			m_Chunks[chunkI][chunkJ]->init(f, _phase);
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
			if (m_Chunks[i][j] != nullptr)
				m_Chunks[i][j]->Render();
}

void MapTile::drawWater()
{
	for (uint32 i = 0; i < C_ChunksInTile; i++)
		for (uint32 j = 0; j < C_ChunksInTile; j++)
			if (m_Chunks[i][j] != nullptr)
				if (m_Chunks[i][j]->m_Liquid != nullptr)
					m_Chunks[i][j]->m_Liquid->draw();
}

void MapTile::drawObjects()
{
#ifdef WMO_INCL
	for (auto it = m_WMOsInstances.begin(); it != m_WMOsInstances.end(); ++it)
	{
		(*it)->Render();
	}
#endif
}

void MapTile::drawSky()
{
#ifdef WMO_INCL
#ifdef MDX_INCL
	for (auto it = m_WMOsInstances.begin(); it != m_WMOsInstances.end(); ++it)
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
	for (auto it = m_MDXsInstances.begin(); it != m_MDXsInstances.end(); ++it)
	{
		(*it)->Render();
	}
#endif
}

//



MapChunk* MapTile::getChunk(uint32 x, uint32 z)
{
	assert1(x < C_ChunksInTile && z < C_ChunksInTile);
	return m_Chunks[x][z];
}
