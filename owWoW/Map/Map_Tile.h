#pragma once

#include "Map_Chunk.h"

#include "Model_Instance.h"
#include "WMO_Instance.h"

enum load_phases;

struct Map_Tile_TextureInfo
{
    Map_Tile_TextureInfo():
        diffuseTexture(nullptr),
        specularTexture(nullptr)
    {
        mtxf.do_not_load_specular_or_height_texture_but_use_cubemap = false;
    }

    //

    string textureName;
    struct 
    {
        uint32_t do_not_load_specular_or_height_texture_but_use_cubemap : 1;
        uint32_t : 31;
    } mtxf;
    Texture* diffuseTexture;
    Texture* specularTexture;
};

class MapTile
{
public:
	MapTile(uint32 _intexX, uint32 _intexZ);
	~MapTile();

	//

	bool Load(cstring _filename);
	bool Load_SplitFile(cstring _filename, load_phases _phase);

	//

	void draw();
	void drawWater();
	void drawObjects();
	void drawSky();
	void drawModels();

	//

	MapChunk* getChunk(uint32 x, uint32 z)
    {
        assert1(x < C_ChunksInTile && z < C_ChunksInTile);
        return m_Chunks[x * C_ChunksInTile + z];
    }

public:
    int                          m_IndexX, m_IndexZ;
    float                        m_GamePositionX, m_GamePositionZ;

	vector<Map_Tile_TextureInfo> m_Textures;

	vector<string>               m_WMOsNames;
    vector<WMOPlacementInfo*>    m_WMOsPlacementInfo;
	vector<WMOInstance*>         m_WMOsInstances;

	vector<string>               m_MDXsNames;
    vector<ModelPlacementInfo*>  m_MDXsPlacementInfo;
	vector<ModelInstance*>       m_MDXsInstances;

    vector<MapChunk*>            m_Chunks;
    vector<Liquid*>              m_MH2O;
};





