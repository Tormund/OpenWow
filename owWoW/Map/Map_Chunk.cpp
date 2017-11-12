#include "../stdafx.h"

// Include
#include "Map_Tile.h"

// General
#include "Map_Chunk.h"

// Additional
#include "Map.h"
#include "Map_Shared.h"

//

MapChunk::MapChunk(MapTile* _parentTile) :
    m_ParentTile(_parentTile),
    m_GamePositionX(0),
    m_GamePositionY(0),
    m_GamePositionZ(0),
    areaID(-1),
    visible(false),
    hasholes(false),
    m_BlendRBGShadowATexture(0),
    m_Indexes(nullptr),
    m_IndexesCount(0),
    m_Liquid(nullptr)
{

    waterlevel[0] = 0;
    waterlevel[1] = 0;

    m_BlendRBGShadowATexture = 0;


    colorBufferEnable = false;

}

MapChunk::~MapChunk()
{
    if (m_Liquid != nullptr)
    {
        delete m_Liquid;
    }
}

//

void MapChunk::Load(File& f, load_phases phase)
{
    size_t startPosition = f.GetPos();

    // Root file. All offsets are correct
    if (phase == main_file)
    {
        // Read header
        f.ReadBytes(&header, 0x80);

        areaID = header.areaid;

        m_GamePositionX = header.xpos;
        m_GamePositionY = header.ypos;
        m_GamePositionZ = header.zpos;

        // correct the x and z values
        m_GamePositionX = m_GamePositionX * (-1.0f) + C_ZeroPoint;
        m_GamePositionZ = m_GamePositionZ * (-1.0f) + C_ZeroPoint;

        hasholes = (header.holes != 0);

        if (m_Liquid != nullptr)
        {
            m_Liquid->createBuffer(vec3(m_GamePositionX, 0.0f, m_GamePositionZ));
        }

        //

        // Init main block

        vec3 tempVertexes[C_MapBufferSize];
        vec3 tempNormals[C_MapBufferSize];
        vec4 mccvColors[C_MapBufferSize];
        vec4 mclvColors[C_MapBufferSize];

        // MCVT sub-chunk (height's)
        f.Seek(startPosition + header.ofsHeight);
        {
            vec3* ttv = tempVertexes;

            vec3 vmin = vec3(m_GamePositionX, Math::MaxFloat, m_GamePositionZ);
            vec3 vmax = vec3(m_GamePositionX + C_ChunkSize, Math::MinFloat, m_GamePositionZ + C_ChunkSize);

            // vertices
            for (uint32 j = 0; j < 17; j++)
            {
                for (uint32 i = 0; i < ((j % 2) ? 8 : 9); i++)
                {
                    float h;
                    f.ReadBytes(&h, sizeof(float));

                    float xpos = i * C_UnitSize;
                    float zpos = j * 0.5f * C_UnitSize;
                    if (j % 2)
                    {
                        xpos += C_UnitSize * 0.5f;
                    }

                    vec3 v = vec3(m_GamePositionX + xpos, m_GamePositionY + h, m_GamePositionZ + zpos);
                    *ttv++ = v;

                    vmin.y = minf(v.y, vmin.y);
                    vmax.y = maxf(v.y, vmax.y);
                }
            }
            m_Bounds.set(vmin, vmax, false);
        }

        // MCNR sub-chunk (normals)
        f.Seek(startPosition + header.ofsNormal);
        {
            // Normal vectors for each corresponding vector above. Its followed by some weird unknown data which is not included in the chunk itself and might be some edge flag bitmaps.
            vec3* ttn = tempNormals;
            for (int j = 0; j < 17; j++)
            {
                for (uint32 i = 0; i < ((j % 2) ? 8 : 9); i++)
                {
                    int8 nor[3];
                    f.ReadBytes(nor, 3);

                    *ttn++ = vec3(-(float)nor[1] / 127.0f, (float)nor[2] / 127.0f, -(float)nor[0] / 127.0f);
                }
            }
        }

        // MCSE sub-chunk (sound emitters)
        f.Seek(startPosition + header.ofsSndEmitters);

        // MCLQ sub-chunk (liquids)
        f.Seek(startPosition + header.ofsLiquid);
        {
            if (header.sizeLiquid)
            {
                Log::Green("MapChunk[%d, %d]: Contain liquid!!!!", m_ParentTile->m_IndexX, m_ParentTile->m_IndexZ);
                fail2("Not implemented");

                //CRange height;
                //f.ReadBytes(&height, 8);
                // m_Liquid = new Liquid(8, 8, vec3(m_GamePositionX, height.min, m_GamePositionZ));
                //m_Liquid->initFromTerrainMCLQ(f, GetLiquidType());
            }
        }

        // MCCV sub-chunk (vertex shading)
        f.Seek(startPosition + header.ofsMCCV);
        {
            vec4* ttn = mccvColors;
            for (int j = 0; j < 17; j++)
            {
                for (uint32 i = 0; i < ((j % 2u) ? 8u : 9u); i++)
                {
                    uint8 nor[4];
                    f.ReadBytes(&nor, sizeof(uint32));

                    *ttn++ = vec4((float)nor[2] / 127.0f, (float)nor[1] / 127.0f, (float)nor[0] / 127.0f, (float)nor[3] / 127.0f);
                }
            }
        }

        // MCLV sub-chunk (lighting)
        f.Seek(startPosition + header.ofsMCLV);
        {
            // Check existst
            f.SeekRelative(-8);
            char blockName[5];
            f.ReadBytes(&blockName, 4);
            flipcc(blockName);
            blockName[4] = 0x00;

            uint32 _ssize;
            f.ReadBytes(&_ssize, 4);

            MCLV_exists = strncmp(blockName, "MCLV", 4) == 0;

            vec4* ttn = mclvColors;
            for (int j = 0; j < 17; j++)
            {
                for (int i = 0; i < ((j % 2) ? 8 : 9); i++)
                {
                    int8 nor[4];
                    f.ReadBytes(&nor, 4);
                    *ttn++ = vec4((float)nor[2] / 255.0f, (float)nor[1] / 255.0f, (float)nor[0] / 255.0f, (float)nor[3] / 255.0f);
                }
            }
        }

        ////////////////////////////

        uint32 t = C_MapBufferSize * sizeof(float);

        // Vertex buffer
        uint32 __vb = _Render->r->createVertexBuffer(18 * t, nullptr);

        _Render->r->updateBufferData(__vb, 0 * t, C_MapBufferSize * sizeof(vec3), tempVertexes);
        _Render->r->updateBufferData(__vb, 3 * t, C_MapBufferSize * sizeof(vec3), tempNormals);
        _Render->r->updateBufferData(__vb, 6 * t, C_MapBufferSize * sizeof(vec4), mccvColors);
        _Render->r->updateBufferData(__vb, 10 * t, C_MapBufferSize * sizeof(vec4), mclvColors);
        _Render->r->updateBufferData(__vb, 14 * t, C_MapBufferSize * sizeof(vec2), Map_Shared::GetTextureCoordDetail());
        _Render->r->updateBufferData(__vb, 16 * t, C_MapBufferSize * sizeof(vec2), Map_Shared::GetTextureCoordAlpha());
        

        //

        __geom = _Render->r->beginCreatingGeometry(_RenderStorage->__layout_GxVBF_PNC2T2);

        _Render->r->setGeomVertexParams(__geom, __vb, R_DataType::T_FLOAT, 0 * t, 0);
        _Render->r->setGeomVertexParams(__geom, __vb, R_DataType::T_FLOAT, 3 * t, 0);
        _Render->r->setGeomVertexParams(__geom, __vb, R_DataType::T_FLOAT, 6 * t, 0);
        _Render->r->setGeomVertexParams(__geom, __vb, R_DataType::T_FLOAT, 10 * t, 0);
        _Render->r->setGeomVertexParams(__geom, __vb, R_DataType::T_FLOAT, 14 * t, 0);
        _Render->r->setGeomVertexParams(__geom, __vb, R_DataType::T_FLOAT, 16 * t, 0);

        vector<uint16>& mapArray = Map_Shared::GenarateDefaultMapArray(header.holes);
        m_Indexes = mapArray.data();
        m_IndexesCount = mapArray.size();

        uint32 __ib = _Render->r->createIndexBuffer(m_IndexesCount * sizeof(uint16), m_Indexes);
        _Render->r->setGeomIndexParams(__geom, __ib, R_IndexFormat::IDXFMT_16);

        _Render->r->finishCreatingGeometry(__geom);
    }

    // Textures file. Offsets are NOT set
    if (phase == tex)
    {
        // Init offsets
        {
            char fcc[5];
            fcc[4] = 0;

            uint32 size;

            f.SeekRelative(-4);
            f.ReadBytes(&size, 4);

            size_t lastpos = f.GetPos() + size;

            while (f.GetPos() < lastpos)
            {
                f.ReadBytes(fcc, 4);
                f.ReadBytes(&size, 4);
                flipcc(fcc);

                size_t nextpos = f.GetPos() + size;

                if (strncmp(fcc, "MCLY", 4) == 0)
                {
                    header.ofsLayer = f.GetPos();
                    header.nLayers = size / 16;
                }
                else if (strncmp(fcc, "MCAL", 4) == 0)
                {
                    if ((size + 8) != header.sizeAlpha)
                    {
                        nextpos = startPosition + header.ofsAlpha + header.sizeAlpha;
                    }

                    header.ofsAlpha = f.GetPos();
                }
                else if (strncmp(fcc, "MCSH", 4) == 0)
                {
                    header.ofsShadow = f.GetPos();
                }

                f.Seek(nextpos);
            }
        }



        //------------------------------------------- Blend buffer Init
        uint8* blendbuf = new uint8[64 * 64 * 4];
        memset(blendbuf, 0, 64 * 64 * 4);
        //------------------------------------------

        // MCLY sub-chunk (m_DiffuseTextures)
        f.Seek(header.ofsLayer);
        {
            // Texture layer definitions for this map chunk. 16 bytes per layer, up to 4 layers (thus, layer count = size / 16).
            for (uint32 i = 0; i < header.nLayers; i++)
            {
                f.ReadBytes(&mcly[i], 16);

                if (mcly[i].flags.animation_enabled)
                {
                    //animated[i] = mcly[i].flags;
                    Log::Error("ANIMATED!!!");
                }
                else
                {
                    animated[i] = 0;
                }
            }
        }

        // MCAL sub-chunk (alpha maps)
        f.Seek(header.ofsAlpha);
        {
            for (uint32 i = 1; i < header.nLayers; i++)
            {
                if (!(mcly[i].flags.use_alpha_map))
                {
                    continue;
                }

                uint8 amap[64 * 64];
                memset(amap, 0x00, 64 * 64);
                uint8* abuf = f.GetDataFromCurrent() + mcly[i].offsetInMCAL;

                if (mcly[i].flags.alpha_map_compressed) // Compressed: MPHD is only about bit depth!
                {
                    assert1(_Map.GetMapFlag().Flag_8bitMCAL);

                    // compressed
                    const uint8* input = abuf;

                    for (uint16 offset_output = 0; offset_output < 4096;)
                    {
                        const bool fill = *input & 0x80;
                        const uint16  n = *input & 0x7F;
                        ++input;

                        if (fill)
                        {
                            memset(&amap[offset_output], *input, n);
                            ++input;
                        }
                        else
                        {
                            memcpy(&amap[offset_output], input, n);
                            input += n;
                        }

                        offset_output += n;
                    }
                }
                else if (_Map.GetMapFlag().Flag_8bitMCAL) // Uncomressed (4096)
                {
                    memcpy(amap, abuf, 64 * 64);
                }
                else // Uncompressed (2048)
                {
                    for (uint8 x = 0; x < 64; ++x)
                    {
                        for (uint8 y = 0; y < 64; y += 2)
                        {
                            amap[x * 64 + y + 0] = ((*abuf & 0x0f) << 4) | (*abuf & 0x0f);
                            amap[x * 64 + y + 1] = ((*abuf & 0xf0) >> 4) | (*abuf & 0xf0);
                            ++abuf;
                        }
                    }

                    if (header.flags.do_not_fix_alpha_map)
                    {
                        for (uint8 i = 0; i < 64; ++i)
                        {
                            amap[i * 64 + 63] = amap[i * 64 + 62];
                            amap[63 * 64 + i] = amap[62 * 64 + i];
                        }
                        amap[63 * 64 + 63] = amap[62 * 64 + 62];
                    }
                }

                for (int p = 0; p < 64 * 64; p++)
                {
                    blendbuf[p * 4 + (i - 1)] = amap[p];
                }
            }
        }

        // MCSH sub-chunk (shadow maps)
        if (header.flags.has_mcsh)
        {
            f.Seek(header.ofsShadow);

            uint8 sbuf[64 * 64], *p, c[8];
            p = sbuf;
            for (int j = 0; j < 64; j++)
            {
                f.ReadBytes(c, 8);
                for (int i = 0; i < 8; i++)
                {
                    for (int b = 0x01; b != 0x100; b <<= 1)
                    {
                        *p++ = (c[i] & b) ? 85 : 0;
                    }
                }
            }

            for (int p = 0; p < 64 * 64; p++)
            {
                blendbuf[p * 4 + 3] = sbuf[p];
            }
        }


        //-------------------------------------------- Blend buffer Final
        m_BlendRBGShadowATexture = _Render->r->createTexture(R_TextureTypes::Tex2D, 64, 64, 1, R_TextureFormats::RGBA8, false, false, false, false);
        _Render->r->uploadTextureData(m_BlendRBGShadowATexture, 0, 0, blendbuf);
        //------------------------------------------
    }

    // Object file. Offset are NOT set
    if (phase == obj)
    {
        // MCRF sub-chunk (???)
        f.Seek(header.ofsRefs);
    }
}

void MapChunk::Post_Load()
{
    // Texture layer definitions for this map chunk. 16 bytes per layer, up to 4 layers (thus, layer count = size / 16).
    for (uint32 i = 0; i < header.nLayers; i++)
    {
        m_DiffuseTextures[i] = m_ParentTile->m_Textures[mcly[i].textureIndex].diffuseTexture;
        m_SpecularTextures[i] = m_ParentTile->m_Textures[mcly[i].textureIndex].specularTexture;
    }
}

//

void MapChunk::drawPass(int anim)
{
    /*if (anim)
    {
        glActiveTexture(GL_TEXTURE0);
        glMatrixMode(GL_TEXTURE);
        glPushMatrix();

        // note: this is ad hoc and probably completely wrong
        int spd = (anim & 0x08) | ((anim & 0x10) >> 2) | ((anim & 0x20) >> 4) | ((anim & 0x40) >> 6);
        int dir = anim & 0x07;
        const float texanimxtab[8] = {0, 1, 1, 1, 0, -1, -1, -1};
        const float texanimytab[8] = {1, 1, 0, -1, -1, -1, 0, 1};
        float fdx = -texanimxtab[dir], fdy = texanimytab[dir];

        int animspd = (int)(200.0f * C_DetailSize);
        float f = (((int)(_TimeManager->animtime * (spd / 15.0f))) % animspd) / (float)animspd;
        glTranslatef(f * fdx, f * fdy, 0);
    }*/

    //glDrawElements(GL_TRIANGLE_STRIP, striplen, GL_UNSIGNED_SHORT, m_Indexes);

    /*if (anim)
    {
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glActiveTexture(GL_TEXTURE1);
    }*/
}


void MapChunk::Render()
{
    visible = false;

    if (_CameraFrustum->_frustum.cullBox(m_Bounds))
    {
        return;
    }

    // Draw chunk before fog
    /*float mydist = (_Camera->Position - vcenter).length() - r;
    if (mydist > _Config.culldistance)
    {
        if (_Config.uselowlod)
        {
            this->drawNoDetail();
            return;
        }
    }*/

    visible = true;

    _TechniquesMgr->m_MapChunk_GeometryPass->SetLayersCount(header.nLayers);

    _TechniquesMgr->m_MapChunk_GeometryPass->SetMCCVExists(header.flags.has_mccv && _Config.Quality.Terrain_MCCV);
    _TechniquesMgr->m_MapChunk_GeometryPass->SetMCLVExists(MCLV_exists && _Config._Config.Quality.Terrain_MCLV);

    _Render->r->setGeometry(__geom);

    //_Render->r->setFillMode(R_FillMode::RS_FILL_WIREFRAME);

    // Bind m_DiffuseTextures
    for (uint32 i = 0; i < header.nLayers; i++)
    {
        _Render->r->setTexture(i, m_DiffuseTextures[i]->GetObj(), _Config.Quality.Texture_Sampler | SS_ADDR_WRAP, 0);
        _Render->r->setTexture(5 + i, m_SpecularTextures[i]->GetObj(), _Config.Quality.Texture_Sampler | SS_ADDR_WRAP, 0);
    }

    // Bind blend
    if (header.nLayers > 0)
    {
        _Render->r->setTexture(4, m_BlendRBGShadowATexture, SS_ADDR_CLAMP, 0);
    }

    // Bind shadow
    _TechniquesMgr->m_MapChunk_GeometryPass->SetShadowMapExists(header.flags.has_mcsh);
    if (header.flags.has_mcsh)
    {
        _TechniquesMgr->m_MapChunk_GeometryPass->SetShadowColor(vec3(0.0f, 0.0f, 0.0f) * 0.3f);
    }

    _Render->r->drawIndexed(PRIM_TRILIST, 0, m_IndexesCount, 0, C_MapBufferSize);

    //PERF_INC(PERF_MAP_CHUNK_GEOMETRY);


    _Render->r->setFillMode(R_FillMode::RS_FILL_SOLID);
}

//

void MapChunk::CreateMH2OLiquid(File& f, MH2O_Header* _liquidHeader)
{
    assert1(_liquidHeader != nullptr);
    assert1(m_Liquid == nullptr);

    m_Liquid = new Liquid(8, 8, vec3());
    m_Liquid->initFromTerrainMH2O(f, _liquidHeader);
}

MCNK_MCLQ_LiquidType MapChunk::GetLiquidType()
{
    if (header.flags.lq_river)
        return MCNK_MCLQ_LiquidType::lq_river;
    else if (header.flags.lq_ocean)
        return MCNK_MCLQ_LiquidType::lq_ocean;
    else if (header.flags.lq_magma)
        return MCNK_MCLQ_LiquidType::lq_magma;
    else if (header.flags.lq_slime)
        return MCNK_MCLQ_LiquidType::lq_slime;
    else
        fail1();
}

