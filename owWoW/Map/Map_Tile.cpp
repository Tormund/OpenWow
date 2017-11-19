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
        uint32 Flag_HasMFBO : 1;
        uint32 Flag_IsNortrend : 1;
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

    uint8 unk0[16];
};

MapTile::MapTile(uint32 _intexX, uint32 _intexZ) : m_IndexX(_intexX), m_IndexZ(_intexZ)
{
    m_GamePositionX = _intexX * C_TileSize;
    m_GamePositionZ = _intexZ * C_TileSize;

    /*for (uint32 i = 0; i < C_ChunksInTileGlobal; i++)
    {
        m_Chunks.push_back(new MapChunk(this));
    }*/
}

MapTile::~MapTile()
{
    Log::Info("MapTile[%d, %d]: Unloading tile...", m_IndexX, m_IndexZ);

    //---------------------------------------------------------------------------------

    ERASE_VECTOR(m_Chunks);

    for (auto it : m_Textures)
    {
        _TexturesMgr->Delete(it.diffuseTexture);
        _TexturesMgr->Delete(it.specularTexture);
    }

    for (auto it : m_WMOsNames)
    {
        _WMOsMgr->Delete(it);
    }
    ERASE_VECTOR(m_WMOsInstances);

    for (auto it : m_MDXsNames)
    {
        _ModelsMgr->Delete(it);
    }
    ERASE_VECTOR(m_MDXsInstances);

    //---------------------------------------------------------------------------------

    Log::Green("MapTile[%d, %d]: Unloaded.", m_IndexX, m_IndexZ);
}

bool MapTile::Load(cstring _filename)
{
    //Log::Info("MapTile[%d, %d, %s]: Loading...", m_IndexX, m_IndexZ, _filename.c_str());

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

    // Load m_DiffuseTextures
    for (auto& it : m_Textures)
    {
        if (it.mtxf.do_not_load_specular_or_height_texture_but_use_cubemap)
        {
            it.diffuseTexture = _TexturesMgr->DefaultTexture();
            it.specularTexture = _TexturesMgr->DefaultTexture();
            continue;
        }

        // Preload diffuse texture
        it.diffuseTexture = _TexturesMgr->Add(it.textureName);

        // Preload specular texture
        string specularTextureName = it.textureName;
        specularTextureName = specularTextureName.insert(specularTextureName.length() - 4, "_s");
        it.specularTexture = _TexturesMgr->Add(specularTextureName);
    }

    // WMOs
    for (auto it : m_WMOsPlacementInfo)
    {
        _WMOsMgr->Add(m_WMOsNames[it->nameIndex]);

        WMO* wmo = (WMO*)_WMOsMgr->objects[m_WMOsNames[it->nameIndex]];
        WMOInstance* inst = new WMOInstance(wmo, it);
        m_WMOsInstances.push_back(inst);
    }

    // MDXs
    for (auto it : m_MDXsPlacementInfo)
    {
        _ModelsMgr->Add(m_MDXsNames[it->nameId]);

        MDX* mdx = (MDX*)_ModelsMgr->GetItemByName(m_MDXsNames[it->nameId]);
        ModelInstance* inst = new ModelInstance(mdx, it);
        m_MDXsInstances.push_back(inst);
    }

    // Chunks
    assert1(m_Chunks.size() == C_ChunksInTileGlobal);
    for (auto it : m_Chunks)
    {
        it->Post_Load();
    }

    //---------------------------------------------------------------------------------

    Log::Green("MapTile[%d, %d, %s]: Loaded!", m_IndexX, m_IndexZ, _filename.c_str());

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
        Log::Error("MapTile[%d, %d, %s]: Error open file!", m_IndexX, m_IndexZ, name);
        return false;
    }

    int chunksCntr = 0;

    char fourcc[5];
    uint32 size;
    while (!f.IsEof())
    {
        memset(fourcc, 0, 4);
        f.ReadBytes(fourcc, 4);
        flipcc(fourcc);
        fourcc[4] = 0;
        f.ReadBytes(&size, 4);
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
        else if (strncmp(fourcc, "MTEX", 4) == 0) // List of m_DiffuseTextures used for texturing the terrain in this map m_TileExists.
        {
            WOWCHUNK_READ_STRINGS_BEGIN

                Map_Tile_TextureInfo textureInfo;
            textureInfo.textureName = _string;

            m_Textures.push_back(textureInfo);

            WOWCHUNK_READ_STRINGS_END
        }
        else if (strncmp(fourcc, "MMDX", 4) == 0) // List of filenames for M2 models that appear in this map m_TileExists.
        {
            WOWCHUNK_READ_STRINGS_BEGIN
                m_MDXsNames.push_back(_string);
            WOWCHUNK_READ_STRINGS_END
        }
        else if (strncmp(fourcc, "MMID", 4) == 0) // List of offsets of model filenames in the MMDX chunk.
        {
            assert1(m_MDXsNames.size() == size / sizeof(uint32));
        }
        else if (strncmp(fourcc, "MWMO", 4) == 0) // List of filenames for WMOs (world map objects) that appear in this map m_TileExists.
        {
            WOWCHUNK_READ_STRINGS_BEGIN
                m_WMOsNames.push_back(_string);
            WOWCHUNK_READ_STRINGS_END
        }
        else if (strncmp(fourcc, "MWID", 4) == 0) // List of offsets of WMO filenames in the MWMO chunk.
        {
            assert1(m_WMOsNames.size() == size / sizeof(uint32));
        }
        else if (strncmp(fourcc, "MDDF", 4) == 0) // Placement information for doodads (M2 models).
        {
            for (uint32 i = 0; i < size / ModelPlacementInfo::__size; i++)
            {
                ModelPlacementInfo* placementInfo = new ModelPlacementInfo();
                f.ReadBytes(placementInfo, ModelPlacementInfo::__size);
                m_MDXsPlacementInfo.push_back(placementInfo);
            }
        }
        else if (strncmp(fourcc, "MODF", 4) == 0) // Placement information for WMOs.
        {
            for (uint32 i = 0; i < size / WMOPlacementInfo::__size; i++)
            {
                WMOPlacementInfo* placementInfo = new WMOPlacementInfo();
                f.ReadBytes(placementInfo, WMOPlacementInfo::__size);
                m_WMOsPlacementInfo.push_back(placementInfo);
            }
        }
        else if (strncmp(fourcc, "MH2O", 4) == 0) // Water
        {
            uint8* abuf = f.GetDataFromCurrent();

            for (uint32 i = 0; i < C_ChunksInTile; i++)
            {
                for (uint32 j = 0; j < C_ChunksInTile; j++)
                {
                    MH2O_Header* mh2o_Header = (MH2O_Header*)abuf;

                    Liquid* liquid = new Liquid(8, 8, vec3());
                    liquid->initFromTerrainMH2O(f, mh2o_Header);
                    liquid->createBuffer(vec3(m_GamePositionX + j * C_ChunkSize, 0.0f, m_GamePositionZ + i * C_ChunkSize));
                    m_MH2O.push_back(liquid);

                    abuf += sizeof(MH2O_Header);
                }
            }
        }
        else if (strncmp(fourcc, "MCNK", 4) == 0)
        {
            if (_phase == load_phases::main_file)
            {
                MapChunk* chunk = new MapChunk(this);
                m_Chunks.push_back(chunk);
            }

            m_Chunks[chunksCntr++]->Load(f, _phase);
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
            assert1(m_Textures.size() == size / sizeof(uint32));

            for (uint32 i = 0; i < size / sizeof(uint32); i++)
            {
                f.ReadBytes(&m_Textures[i].mtxf, sizeof(uint32));
            }
        }
        else if (strncmp(fourcc, "MAMP", 4) == 0)
        {
            int8 mamp_value;
            f.ReadBytes(&mamp_value, 1);
            if (mamp_value != 0)
            {
                Log::Print("Mamp value is [%d]", mamp_value);
                Log::Print("Texture size is [%d]", 64 / (pow(2, mamp_value)));
                fail1();
            }
        }
        else
        {
            Log::Info("MapTile[%s]: No implement chunk %s [%d].", name, fourcc, size);
            fail1();
        }

        f.Seek(nextpos);
    }

    return true;
}

//

void MapTile::draw()
{
    for (auto it : m_Chunks)
    {
        it->Render();
    }
}

void MapTile::Render_DEBUG()
{
    for (auto it : m_Chunks)
    {
        it->Render_DEBUG();
    }
}

void MapTile::drawWater()
{
    for (auto it : m_MH2O)
    {
        it->draw();
    }
}

void MapTile::drawObjects()
{
    for (auto it : m_WMOsInstances)
    {
        it->Render();
    }
}

void MapTile::drawSky()
{
    for (auto it : m_WMOsInstances)
    {
        it->GetWMO()->drawSkybox();

        if (_EnvironmentManager->m_HasSky)
        {
            break;
        }
    }
}

void MapTile::drawModels()
{
    for (auto it : m_MDXsInstances)
    {
        it->Render();
    }
}
