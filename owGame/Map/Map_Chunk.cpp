#include "../stdafx.h"

// Include
#include "Map_Tile.h"

// General
#include "Map_Chunk.h"

// Additional
#include "Map.h"

struct MCLY
{
	uint32 textureIndex;
	struct
	{
		uint32 animation_rotation : 3;        // each tick is 45
		uint32 animation_speed : 3;
		uint32 animation_enabled : 1;
		uint32 overbright : 1;                // This will make the texture way brighter. Used for lava to make it "glow".
		uint32 use_alpha_map : 1;             // set for every layer after the first
		uint32 alpha_map_compressed : 1;      // see MCAL chunk description
		uint32 use_cube_map_reflection : 1;   // This makes the layer behave like its a reflection of the skybox. See below
		uint32 : 21;
	} flags;
	uint32 offsetInMCAL;
	__DBC_FOREIGN_KEY_ID(uint16, DBC_GroundEffectTexture, effectId);
	int16 padding;
};

bool isHole(int holes, int i, int j)
{
	const int holetab_h[4] = {0x1111, 0x2222, 0x4444, 0x8888};
	const int holetab_v[4] = {0x000F, 0x00F0, 0x0F00, 0xF000};

	return (holes & holetab_h[i] & holetab_v[j]) != 0;
}

//

MapChunk::MapChunk(MapTile* _parentTile) :
	m_ParentTile(_parentTile),
	m_GamePositionX(0),
	m_GamePositionY(0),
	m_GamePositionZ(0),
	areaID(-1),
	visible(false),
	hasholes(false),
	blend(0),
	strip(0),
	striplen(0),
	m_Liquid(nullptr)
{

	waterlevel[0] = 0;
	waterlevel[1] = 0;

	blend = 0;


	colorBufferEnable = false;

}

MapChunk::~MapChunk()
{
	if (hasholes)
	{
		delete[] strip;
	}

	if (m_Liquid != nullptr)
	{
		delete m_Liquid;
	}
}

//

void MapChunk::init(File& f, load_phases phase)
{
	size_t startPosition = f.GetPos();

	// Root file. All offsets are correct
	if (phase == main_file)
	{
		// Read header
		header = new MCNK_Header;
		f.ReadBytes(header, 0x80);

		areaID = header->areaid;

		m_GamePositionX = header->xpos;
		m_GamePositionY = header->ypos;
		m_GamePositionZ = header->zpos;

		// correct the x and z values
		m_GamePositionX = m_GamePositionX * (-1.0f) + C_ZeroPoint;
		m_GamePositionZ = m_GamePositionZ * (-1.0f) + C_ZeroPoint;

		hasholes = (header->holes != 0);

		if (m_Liquid != nullptr)
		{
			m_Liquid->createBuffer(vec3(m_GamePositionX, 0.0f, m_GamePositionZ));
		}

		//

		// Init main block

		vec3 tempVertexes[C_MapBufferSize];
		vec3 tempNormals[C_MapBufferSize];
		vec3 mccvColors[C_MapBufferSize];
		vec4 mclvColors[C_MapBufferSize];

		for (int i = 0; i < C_MapBufferSize; i++)
		{
			mccvColors[i] = vec3(0.0f, 0.0f, 0.0f);
			mclvColors[i] = vec4(0.0f, 0.0f, 0.0f, 0.0f);
		}

		// MCVT sub-chunk (height's)
		f.Seek(startPosition + header->ofsHeight);
		{
			/*
			These are the actual height values for the 9x9+8x8 vertices. 145 floats in the following order/arrangement:.
			1    2	3	 4	  5    6	7	 8	  9
			  10	11	 12   13   14	15	 16   17
			18   19   20	21	 22   23   24	25	 26
			  27	28	 29   30   31	32	 33   34
			35   36   37	38	 39   40   41	42	 43
			  44	45	 46   47   48	49	 50   51
			52   53   54	55	 56   57   58	59	 60
			  61	62	 63   64   65	66	 67   68
			69   70   71	72	 73   74   75	76	 77
			  78	79	 80   81   82	83	 84   85
			86   87   88	89	 90   91   92	93	 94
			  95	96	 97   98  99  100  101	102
			103  104  105  106	107  108  109  110	111
			  112  113	114  115  116  117	118  119
			120  121  122  123	124  125  126  127	128
			  129  130	131  132  133  134	135  136
			137  138  139  140	141  142  143  144	145
			The inner 8 vertices are only rendered in WoW when its using the up-close LoD. Otherwise, it only renders the outer 9. Nonsense? If I only change one of these it looks like: [1].
			Ok, after a further look into it, WoW uses Squares out of 4 of the Outer(called NoLoD)-Vertices with one of the Inner(called LoD)-Vertices in the Center:
			*/
			vec3 *ttv = tempVertexes;

			vec3 vmin, vmax;

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

					if (v.y < vmin.y)
					{
						vmin.y = v.y;
					}

					if (v.y > vmax.y)
					{
						vmax.y = v.y;
					}
				}
			}

			vmin.x = m_GamePositionX;
			vmin.z = m_GamePositionZ;
			vmax.x = m_GamePositionX + C_ChunkSize;
			vmax.z = m_GamePositionZ + C_ChunkSize;

			m_Bounds.set(vmin, vmax, false);

		}

		// MCNR sub-chunk (normals)
		f.Seek(startPosition + header->ofsNormal);
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
		f.Seek(startPosition + header->ofsSndEmitters);

		// MCLQ sub-chunk (liquids)
		f.Seek(startPosition + header->ofsLiquid); // DEPRECATED
		{
			if (header->sizeLiquid)
			{
				Modules::log().Green("MapChunk[%d, %d]: Contain liquid!!!!", m_ParentTile->m_IndexX, m_ParentTile->m_IndexZ);
				fail1();

				CRange height;
				f.ReadBytes(&height, 8);

				m_Liquid = new Liquid(8, 8, vec3(m_GamePositionX, height.min, m_GamePositionZ));
				m_Liquid->initFromTerrainMCLQ(f, GetLiquidType());
			}
		}

		// MCCV sub-chunk (vertex shading)
		f.Seek(startPosition + header->ofsMCCV);
		{
			vec3* ttn = mccvColors;
			for (int j = 0; j < 17; j++)
			{
				for (uint32 i = 0; i < ((j % 2u) ? 8u : 9u); i++)
				{
					uint8 nor[4];
					f.ReadBytes(nor, 4);
					*ttn++ = vec3((float)nor[2] / 127.0f, (float)nor[1] / 127.0f, (float)nor[0] / 127.0f);
				}
			}
		}

		// MCLV sub-chunk (lighting)
		f.Seek(startPosition + header->ofsMCLV);
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

			CArgb color;
			vec4* ttn = mclvColors;
			for (int j = 0; j < 17; j++)
			{
				for (int i = 0; i < ((j % 2) ? 8 : 9); i++)
				{
					uint8 nor[4];
					f.ReadBytes(nor, 4);
					*ttn++ = vec4((float)nor[2] / 255.0f, (float)nor[1] / 255.0f, (float)nor[0] / 255.0f, (float)nor[3] / 255.0f);
				}
			}
		}

		////////////////////////////

		uint32 t = C_MapBufferSize * sizeof(float);

		// Vertex buffer
		uint32 __vb = _Render->r->createVertexBuffer(17 * t, nullptr);

		_Render->r->updateBufferData(__vb, 0 * t,  C_MapBufferSize * sizeof(vec3), tempVertexes);
		_Render->r->updateBufferData(__vb, 3 * t,  C_MapBufferSize * sizeof(vec2), _Map->GetTextureCoordDetail());
		_Render->r->updateBufferData(__vb, 5 * t,  C_MapBufferSize * sizeof(vec2), _Map->GetTextureCoordAlpha());
		_Render->r->updateBufferData(__vb, 7 * t,  C_MapBufferSize * sizeof(vec3), tempNormals);
		_Render->r->updateBufferData(__vb, 10 * t, C_MapBufferSize * sizeof(vec3), mccvColors);
		_Render->r->updateBufferData(__vb, 13 * t, C_MapBufferSize * sizeof(vec4), mclvColors);

		//

		__geom = _Render->r->beginCreatingGeometry(_Render->__layoutMapChunk);

		// Vertex params
		_Render->r->setGeomVertexParams(__geom, __vb, 0, 0 * t, 0);
		_Render->r->setGeomVertexParams(__geom, __vb, 1, 3 * t, 0);
		_Render->r->setGeomVertexParams(__geom, __vb, 2, 5 * t, 0);
		_Render->r->setGeomVertexParams(__geom, __vb, 3, 7 * t, 0);
		_Render->r->setGeomVertexParams(__geom, __vb, 4, 10 * t, 0);
		_Render->r->setGeomVertexParams(__geom, __vb, 5, 13 * t, 0);

		if (hasholes)
		{
			initStrip(header->holes);
		}
		else
		{
			strip = _Map->GetHighResolutionIndexes();
			striplen = _Map->C_HighResStripSize;
		}

		// Index bufer
		uint32 __ib = _Render->r->createIndexBuffer(striplen * sizeof(uint16), strip);
		_Render->r->setGeomIndexParams(__geom, __ib, R_IndexFormat::IDXFMT_16);

		// Finish
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
					header->ofsLayer = f.GetPos();
					header->nLayers = size / 16;
				}
				else if (strncmp(fcc, "MCAL", 4) == 0)
				{
					if ((size + 8) != header->sizeAlpha)
					{
						nextpos = startPosition + header->ofsAlpha + header->sizeAlpha;
					}

					header->ofsAlpha = f.GetPos();
				}
				else if (strncmp(fcc, "MCSH", 4) == 0)
				{
					header->ofsShadow = f.GetPos();
				}

				f.Seek(nextpos);
			}
		}

		MCLY mcly[4];

		//------------------------------------------- Blend buffer Init
		uint8* blendbuf = new uint8[64 * 64 * 4];
		memset(blendbuf, 0, 64 * 64 * 4);
		//------------------------------------------

		// MCLY sub-chunk (textures)
		f.Seek(header->ofsLayer);
		{
			// Texture layer definitions for this map chunk. 16 bytes per layer, up to 4 layers (thus, layer count = size / 16).
			for (uint32 i = 0; i < header->nLayers; i++)
			{
				f.ReadBytes(&mcly[i], 16);

				if (mcly[i].flags.animation_enabled)
				{
					//animated[i] = mcly[i].flags;
				}
				else
				{
					animated[i] = 0;
				}

				textures[i] = m_ParentTile->m_DiffuseTextures[mcly[i].textureIndex];
				SpecularTextures[i] = m_ParentTile->m_SpecularTextures[mcly[i].textureIndex];
			}
		}

		// MCAL sub-chunk (alpha maps)
		f.Seek(header->ofsAlpha);
		{
			// alpha maps  64 x 64 = 4096
			uint8* data = f.GetDataFromCurrent();
			if (header->nLayers > 0)
			{
				for (uint32 i = 1; i < header->nLayers; i++)
				{
					if (!(mcly[i].flags.use_alpha_map))
					{
						continue;
					}

					uint8 amap[64 * 64];
					uint8* abuf = data + mcly[i].offsetInMCAL;

					if (_Map->GetMapFlag().ALL_MCNK_MCAL_BIGALPHA && mcly[i].flags.alpha_map_compressed) // Compressed: MPHD is only about bit depth!
					{ 
						unsigned offI = 0; //offset IN buffer
						unsigned offO = 0; //offset OUT buffer

						while (offO < 4096)
						{
							// fill or copy mode
							bool fill = abuf[offI] & 0x80;
							unsigned n = abuf[offI] & 0x7F;
							offI++;
							for (unsigned k = 0; k < n; k++)
							{
								amap[offO] = abuf[offI];
								offO++;
								if (!fill)
									offI++;
							}
							if (fill) offI++;
						}

					}
					else if (_Map->GetMapFlag().ALL_MCNK_MCAL_BIGALPHA) // Uncomressed (4096)
					{ 
						if (f.GetPos() + mcly[i].offsetInMCAL + 0x1000 > f.GetSize())
						{
							Modules::log().Info("CONT");
							continue;
						}

						uint8* p = &amap[0];
						for (int j = 0; j < 64; j++)
						{
							for (int i = 0; i < 64; i++)
							{
								*p++ = (*abuf++);
							}
						}
					}
					else // Uncompressed (2048)
					{ 
						uint8 *p = &amap[0];
						for (int j = 0; j < 64; j++)
						{
							for (int k = 0; k < 32; k++)
							{
								unsigned char c = *abuf++;
								if (i != 31)
								{
									*p++ = (unsigned char)((255 * ((int)(c & 0x0f))) / 0x0f);
									*p++ = (unsigned char)((255 * ((int)(c & 0xf0))) / 0xf0);
								}
								else
								{
									*p++ = (unsigned char)((255 * ((int)(c & 0x0f))) / 0x0f);
									*p++ = (unsigned char)((255 * ((int)(c & 0x0f))) / 0x0f);
								}
							}
						}
					}

					for (int p = 0; p < 64 * 64; p++)
					{
						blendbuf[p * 4 + (i - 1)] = amap[p];
					}
				}
			}
		}

		// MCSH sub-chunk (shadow maps)
		if (header->flags.has_mcsh)
		{
			f.Seek(header->ofsShadow);

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
		blend = _Render->r->createTexture(R_TextureTypes::Tex2D, 64, 64, 1, R_TextureFormats::RGBA8, false, false, false, false);
		_Render->r->uploadTextureData(blend, 0, 0, blendbuf);
		//------------------------------------------
	}

	// Object file. Offset are NOT set
	if (phase == obj)
	{
		// MCRF sub-chunk (???)
		f.Seek(header->ofsRefs);
	}
}

void MapChunk::initStrip(int holes)
{
	strip = new short[256]; // TODO: figure out exact length of strip needed
	short *s = strip;
	bool first = true;
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			if (!isHole(holes, x, y))
			{
				// draw tiles here this is ugly but sort of works
				int i = x * 2;
				int j = y * 4;
				for (int k = 0; k < 2; k++)
				{
					if (!first)
					{
						*s++ = indexMapBuf(i, j + k * 2);
					}
					else
					{
						first = false;
					}

					for (int l = 0; l < 3; l++)
					{
						*s++ = indexMapBuf(i + l, j + k * 2);
						*s++ = indexMapBuf(i + l, j + k * 2 + 2);
					}
					*s++ = indexMapBuf(i + 2, j + k * 2 + 2);
				}
			}
		}
	}
	striplen = (int)(s - strip);
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

	//glDrawElements(GL_TRIANGLE_STRIP, striplen, GL_UNSIGNED_SHORT, strip);

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
	if (mydist > Modules::config().culldistance)
	{
		if (Modules::config().uselowlod)
		{
			this->drawNoDetail();
			return;
		}
	}*/

	visible = true;

	//if (!hasholes)
	//{
		//bool highres = Modules::config().drawhighres;
		//if (highres)
		//{
		//	highres = mydist < Modules::config().highresdistance2;
		//}

		//if (highres)
		//{
		//	strip = _Map->GetHighResolutionIndexes();
		//	striplen = _Map->C_HighResStripSize;
		//}
		//else
		//{
		//strip = _Map->GetLowResolutionIndexes();
		//striplen = _Map->C_LowResStripSize;
		//}
	//}

	_TechniquesMgr->m_MapChunk_GeometryPass->SetLayersCount(header->nLayers);

	_TechniquesMgr->m_MapChunk_GeometryPass->SetMCCVExists(header->flags.has_mccv && Modules::config().enableMCCV);
	_TechniquesMgr->m_MapChunk_GeometryPass->SetMCLVExists(MCLV_exists && Modules::config().enableMCLV);


	/*if (Modules::config().drawhighres)
	{
		_Render->r->setGeomIndexParams(__geom, __ibHigh, R_IndexFormat::IDXFMT_16);
		striplen = _Map->C_HighResStripSize;
	}
	else
	{
		_Render->r->setGeomIndexParams(__geom, __ibLow, R_IndexFormat::IDXFMT_16);
		striplen = _Map->C_LowResStripSize;
	}*/

	_Render->r->setGeometry(__geom);

	//_Render->r->setFillMode(R_FillMode::RS_FILL_WIREFRAME);

	// Bind textures
	for (uint32 i = 0; i < header->nLayers; i++)
	{
		_Render->r->setTexture(i, textures[i]->GetObj(), SS_FILTER_BILINEAR | SS_ANISO16 | SS_ADDR_WRAP, 0);
		_Render->r->setTexture(5 + i, SpecularTextures[i]->GetObj(), SS_FILTER_BILINEAR | SS_ANISO16 | SS_ADDR_WRAP, 0);
	}

	// Bind blend
	if (header->nLayers > 0)
	{
		_Render->r->setTexture(4, blend, SS_FILTER_BILINEAR | SS_ADDR_CLAMP, 0);
	}

	// Bind shadow
	_TechniquesMgr->m_MapChunk_GeometryPass->SetShadowMapExists(header->flags.has_mcsh);
	if (header->flags.has_mcsh)
	{
		_TechniquesMgr->m_MapChunk_GeometryPass->SetShadowColor(vec3(0.0f, 0.0f, 0.0f) * 0.3f);
	}

	_Render->r->drawIndexed(PRIM_TRISTRIP, 0, striplen, 0, C_MapBufferSize);

	PERF_INC(PERF_MAP_CHUNK_GEOMETRY);


	//_Render->r->setFillMode(R_FillMode::RS_FILL_SOLID);
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
	assert1(header != nullptr);

	if (header->flags.lq_river)
		return MCNK_MCLQ_LiquidType::lq_river;
	else if (header->flags.lq_ocean)
		return MCNK_MCLQ_LiquidType::lq_ocean;
	else if (header->flags.lq_magma)
		return MCNK_MCLQ_LiquidType::lq_magma;
	else if (header->flags.lq_slime)
		return MCNK_MCLQ_LiquidType::lq_slime;
}

