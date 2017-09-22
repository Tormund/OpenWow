#include "stdafx.h"

// Include
#include "liquid.h"

// General
#include "MapChunk.h"

// Additional
#include "MapTile.h"
#include "world.h"

bool isHole(int holes, int i, int j)
{
	const int holetab_h[4] = {0x1111, 0x2222, 0x4444, 0x8888};
	const int holetab_v[4] = {0x000F, 0x00F0, 0x0F00, 0xF000};

	return (holes & holetab_h[i] & holetab_v[j]) != 0;
}

static uint8_t blendbuf[64 * 64 * 4]; // make unstable when new/delete, just make it global

//

MapChunk::MapChunk() :
	m_GamePositionX(0),
	ybase(0),
	m_GamePositionZ(0),
	r(0),
	areaID(-1),
	haswater(false),
	visible(false),
	hasholes(false),
	shadow(0),
	blend(0),
	vertices(0), 
	normals(0), 
	strip(0), 
	striplen(0), 
	lq(0)
{

	waterlevel[0] = 0;
	waterlevel[1] = 0;

	shadow = new Texture();
	blend = new Texture();

	for (int i = 0; i < 3; i++)
	{
		alphamaps[i] = new Texture();
	}

	colorBufferEnable = false;
}

MapChunk::~MapChunk()
{
	//delete[] alphamaps;

	//delete shadow;

	glDeleteBuffers(1, &vertices);
	glDeleteBuffers(1, &normals);

	if (hasholes)
	{
		delete[] strip;
	}

	if (haswater)
	{
		delete lq;
	}
}

//

void MapChunk::init(vector<Texture*>* mt, File& f, load_phases phase)
{
	size_t startPosition = f.GetPos();

	if (phase == main_file)
	{
		// Read header
		header = new MCNK_Header;
		f.ReadBytes(header, 0x80);

		areaID = header->areaid;

		m_GamePositionZ = header->zpos;
		m_GamePositionX = header->xpos;
		ybase = header->ypos;

		// correct the x and z values
		m_GamePositionZ = m_GamePositionZ * (-1.0f) + C_ZeroPoint;
		m_GamePositionX = m_GamePositionX * (-1.0f) + C_ZeroPoint;

		if (supportShaders)
		{
			//blendbuf = new unsigned char[64*64*4];
			memset(blendbuf, 0, 64 * 64 * 4);
		}
	}

	hasholes = (header->holes != 0);

	// Root file. All offsets are correct
	if (phase == main_file)
	{
		vec3 tempVertexes[C_MapBufferSize];
		vec3 tempNormals[C_MapBufferSize];
		CArgb mclvColors[C_MapBufferSize];
		CArgb mccvColors[C_MapBufferSize];

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

			// vertices
			for (int j = 0; j < 17; j++)
			{
				for (int i = 0; i < ((j % 2) ? 8 : 9); i++)
				{
					float h, xpos, zpos;
					f.ReadBytes(&h, 4);
					xpos = i * C_UnitSize;
					zpos = j * 0.5f * C_UnitSize;
					if (j % 2)
					{
						xpos += C_UnitSize*0.5f;
					}
					vec3 v = vec3(m_GamePositionX + xpos, ybase + h, m_GamePositionZ + zpos);
					*ttv++ = v;
					if (v.y < vmin.y)
						vmin.y = v.y;
					if (v.y > vmax.y)
						vmax.y = v.y;
				}
			}

			vmin.x = m_GamePositionX;
			vmin.z = m_GamePositionZ;
			vmax.x = m_GamePositionX + 8 * C_UnitSize;
			vmax.z = m_GamePositionZ + 8 * C_UnitSize;
			r = glm::length(vmax - vmin) * 0.5f;
		}

		// MCNR sub-chunk (normals)
		f.Seek(startPosition + header->ofsNormal);
		{
			// Normal vectors for each corresponding vector above. Its followed by some weird unknown data which is not included in the chunk itself and might be some edge flag bitmaps.
			int8_t nor[3];
			vec3* ttn = tempNormals;
			for (int j = 0; j < 17; j++)
			{
				for (int i = 0; i < ((j % 2) ? 8 : 9); i++)
				{
					f.ReadBytes(nor, 3);
					*ttn++ = vec3(-(float)nor[1] / 127.0f, (float)nor[2] / 127.0f, -(float)nor[0] / 127.0f);
				}
			}
		}

		// MCSE sub-chunk (sound emitters)
		f.Seek(startPosition + header->ofsSndEmitters);

		// MCLQ sub-chunk (liquids)
		f.Seek(startPosition + header->ofsLiquid);
		{
			//haswater = true;
			//f.ReadBytes(&waterlevel, 8); // 2 values - Lowest water Level, Highest Water Level

			//if(waterlevel[1] > vmax.y) vmax.y = waterlevel[1];
			//if (waterlevel < vmin.y) haswater = false;

			//lq = new Liquid(8, 8, vec3(xbase, waterlevel[1], zbase));
			//lq->init(f);
			//lq->initFromTerrain(f, header->flags);
		}

		// MCCV sub-chunk (vertex shading)
		f.Seek(startPosition + header->ofsMCCV);
		{
			// CImVector values[9 * 9 + 8 * 8];
			CImVector color;
			CArgb* ttn = mccvColors;
			for (int j = 0; j < 17; j++)
			{
				for (int i = 0; i < ((j % 2) ? 8 : 9); i++)
				{
					f.ReadBytes(&color, 4);
					*ttn++ = CArgb(color.r, color.g, color.b, color.a);
				}
			}
		}

		// MCLV sub-chunk (lighting)
		f.Seek(startPosition + header->ofsMCLV);
		{
			// Check existst
			f.SeekRelative(-8);
			char blockName[5];
			uint32_t _ssize;
			f.ReadBytes(&blockName, 4);
			f.ReadBytes(&_ssize, 4);
			flipcc(blockName);
			MCLV_exists = strncmp(blockName, "MCLV", 4) == 0;

			// CArgb values[9 * 9 + 8 * 8];
			CArgb color;
			CArgb* ttn = mclvColors;
			for (int j = 0; j < 17; j++)
			{
				for (int i = 0; i < ((j % 2) ? 8 : 9); i++)
				{
					f.ReadBytes(&color, 4);
					*ttn++ = CArgb(color.r, color.g, color.b, color.a); // Alpha is apparently ignored
				}
			}
		}

		CArgb mclvAndMccvColors[C_MapBufferSize];
		//memset(&mclvAndMccvColors, 0, C_MapBufferSize * 4);

		/*if(MCLV_exists && (header->flags & MCNK_Header::FLAG_HAS_MCCV)) {
			for(size_t i = 0; i < C_MapBufferSize; i++)
				mclvAndMccvColors[i] = CArgb((mclvColors[i].r + mccvColors[i].r) / 2, (mclvColors[i].g * mccvColors[i].g) / 2, (mclvColors[i].b * mccvColors[i].b) / 2, 255);
		}
		else *//*if(MCLV_exists) {
			for(size_t i = 0; i < C_MapBufferSize; i++)
				mclvAndMccvColors[i] = CArgb(mclvColors[i].r, mclvColors[i].g, mclvColors[i].b, 128);
		}
		else */if (header->flags & MCNK_Header::FLAG_HAS_MCCV)
		{
			for (size_t i = 0; i < C_MapBufferSize; i++)
				mclvAndMccvColors[i] = CArgb(mccvColors[i].r, mccvColors[i].g, mccvColors[i].b, 128);
		}

		if (/*MCLV_exists || */(header->flags & MCNK_Header::FLAG_HAS_MCCV))
		{
			glGenBuffers(1, &colorBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
			glBufferData(GL_ARRAY_BUFFER, C_MapBufferSize * 4 * sizeof(uint8_t), mclvAndMccvColors, GL_STATIC_DRAW);
		}

		// Buffers
		{
			glGenBuffers(1, &vertices);
			glBindBuffer(GL_ARRAY_BUFFER, vertices);
			glBufferData(GL_ARRAY_BUFFER, C_MapBufferSize * 3 * sizeof(float), tempVertexes, GL_STATIC_DRAW);

			glGenBuffers(1, &normals);
			glBindBuffer(GL_ARRAY_BUFFER, normals);
			glBufferData(GL_ARRAY_BUFFER, C_MapBufferSize * 3 * sizeof(float), tempNormals, GL_STATIC_DRAW);

			if (hasholes)
				initStrip(header->holes);

			if (supportShaders)
			{
				blend->GenerateTexture();
				blend->Bind();
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, blendbuf);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				//delete[] blendbuf;
			}


			// ------------------------------------------------------------------------------------------------------------


			// Vertexes
			/*glGenBuffers(1, &vertexBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			{
				glBufferData(GL_ARRAY_BUFFER, sizeof(tempVertexes), tempVertexes, GL_STATIC_DRAW);

				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), nullptr);
			}
			glBindBuffer(GL_ARRAY_BUFFER, 0);*/


		}
	}

	// Textures file. Offsets are NOT set
	if (phase == tex)
	{
		// Init offsets
		{
			char fcc[5];
			fcc[4] = 0;

			uint32_t size;

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

		struct MCLY
		{
			uint32_t textureIndex;
			/*struct {
				uint32_t animation_rotation : 3;        // each tick is 45°
				uint32_t animation_speed : 3;
				uint32_t animation_enabled : 1;
				uint32_t overbright : 1;                // This will make the texture way brighter. Used for lava to make it "glow".
				uint32_t use_alpha_map : 1;             // set for every layer after the first
				uint32_t alpha_map_compressed : 1;      // see MCAL chunk description
				uint32_t use_cube_map_reflection : 1;   // This makes the layer behave like its a reflection of the skybox. See below
				uint32_t : 21;
			} flags;*/

			enum
			{
				FLAG_USE_ALPHA_MAP = 0x100,
				FLAG_ALPHA_MAP_COMRESSED = 0x200,
				FLAG_CUBE_MAP_REFLECTIONS = 0x400,
			};

			uint32_t flags;
			uint32_t offsetInMCAL;
			int16_t effectId;
			int16_t padding;
		};
		MCLY mcly[4];
		memset(mcly, 0, sizeof(struct MCLY) * 4);

		// MCLY sub-chunk (textures)
		f.Seek(header->ofsLayer);
		{
			// Texture layer definitions for this map chunk. 16 bytes per layer, up to 4 layers (thus, layer count = size / 16).
			for (uint32_t i = 0; i < header->nLayers; i++)
			{
				f.ReadBytes(&mcly[i], 16);

				if (mcly[i].flags & 0x80)
				{
					animated[i] = mcly[i].flags;
				}
				else
				{
					animated[i] = 0;
				}

				textures[i] = (*mt)[mcly[i].textureIndex];
			}
		}

		// MCAL sub-chunk (alpha maps)
		f.Seek(header->ofsAlpha);
		{
			// alpha maps  64 x 64 = 4096
			uint8_t* data = f.GetDataFromCurrent();
			if (header->nLayers > 0)
			{
				for (uint32_t i = 1; i < header->nLayers; i++)
				{
					alphamaps[i - 1]->GenerateTexture();

					if (!((mcly[i].flags & MCLY::FLAG_USE_ALPHA_MAP) == MCLY::FLAG_USE_ALPHA_MAP))
					{
						continue;
					}

					
					alphamaps[i - 1]->Bind();

					uint8_t amap[64 * 64];
					uint8_t* abuf = data + mcly[i].offsetInMCAL;

					if (_World->GetMap()->IsBigAlpha() && ((mcly[i].flags & MCLY::FLAG_ALPHA_MAP_COMRESSED) == MCLY::FLAG_ALPHA_MAP_COMRESSED))
					{ // Compressed: MPHD is only about bit depth!
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
					else if (_World->GetMap()->IsBigAlpha())
					{ // Uncomressed (4096)
						if (f.GetPos() + mcly[i].offsetInMCAL + 0x1000 > f.GetSize())
						{
							Debug::Info("CONT");
							continue;
						}

						uint8_t* p = &amap[0];
						for (int j = 0; j < 64; j++)
						{
							for (int i = 0; i < 64; i++)
							{
								*p++ = (*abuf++);
							}
						}
					}
					else
					{ // Uncompressed (2048)
						uint8_t *p = &amap[0];
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

					glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 64, 64, 0, GL_ALPHA, GL_UNSIGNED_BYTE, amap);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					alphamaps[i - 1]->Unbind();

					if (supportShaders)
					{
						for (int p = 0; p < 64 * 64; p++)
						{
							blendbuf[p * 4 + i - 1] = amap[p];
						}
					}
				}

			}
		}

		// MCSH sub-chunk (shadow maps)
		if (header->flags & MCNK_Header::FLAG_HAS_MCSH == MCNK_Header::FLAG_HAS_MCSH)
		{
			f.Seek(header->ofsShadow);

			uint8_t sbuf[64 * 64], *p, c[8];
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

			shadow->GenerateTexture();
			shadow->Bind();
			glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 64, 64, 0, GL_ALPHA, GL_UNSIGNED_BYTE, sbuf);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			shadow->Unbind();

			if (supportShaders)
			{
				for (int p = 0; p < 64 * 64; p++)
				{
					blendbuf[p * 4 + 3] = sbuf[p];
				}
			}
		}
	}

	// Object file. Offset are NOT set
	if (phase == obj)
	{
		// MCRF sub-chunk (???)
		f.Seek(header->ofsRefs);
	}

	//

	vcenter = (vmin + vmax) * 0.5f;
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
				// draw m_TileExists here
				// this is ugly but sort of works
				int i = x * 2;
				int j = y * 4;
				for (int k = 0; k < 2; k++)
				{
					if (!first)
					{
						*s++ = indexMapBuf(i, j + k * 2);
					}
					else first = false;
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
	if (anim)
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

		int animspd = (int)(200.0f * detail_size);
		float f = (((int)(_World->animtime*(spd / 15.0f))) % animspd) / (float)animspd;
		glTranslatef(f * fdx, f * fdy, 0);
	}

	glDrawElements(GL_TRIANGLE_STRIP, striplen, GL_UNSIGNED_SHORT, strip);

	if (anim)
	{
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glActiveTexture(GL_TEXTURE1);
	}
}

void MapChunk::draw()
{
	if (!_World->frustum.intersects(vmin, vmax))
	{
		return;
	}

	if (header->nLayers == 0)
	{
		return;
	}

	float mydist = glm::length(_Camera->Position - vcenter) - r;

	if (mydist > _WowSettings->culldistance)
	{
		if (_WowSettings->uselowlod)
		{
			this->drawNoDetail();
		}
		return;
	}

	visible = true;

	if (!hasholes)
	{
		bool highres = _WowSettings->drawhighres;
		if (highres)
		{
			highres = mydist < _WowSettings->highresdistance2;
		}

		if (highres)
		{
			strip = _World->mapstrip2;
			striplen = stripsize2;
		}
		else
		{
			strip = _World->mapstrip;
			striplen = stripsize;
		}
	}

	// setup vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, normals);
	glNormalPointer(GL_FLOAT, 0, 0);



	if (supportShaders && _WowSettings->useshaders)
	{
		/*
		unit 0 - base texture layer
		unit 1 - shadow map and alpha layers
		unit 2 - texture layer 1
		unit 3 - texture layer 2
		unit 4 - texture layer 3
		*/
		// base layer
		glActiveTextureARB(GL_TEXTURE0_ARB);
		textures[0]->Bind();

		// shadow map
		// TODO: handle case when there is no shadowmap?
		glActiveTextureARB(GL_TEXTURE1_ARB);
		blend->Bind();

		// blended layers
		for (uint32_t i = 1; i < header->nLayers; i++)
		{
			int tex = GL_TEXTURE2_ARB + i - 1;
			glActiveTextureARB(tex);
			textures[i]->Bind();
		}
		glActiveTextureARB(GL_TEXTURE0_ARB);

		terrainShaders[header->nLayers - 1]->bind();

		vec3 shc = _World->skies->colorSet[SHADOW_COLOR] * 0.3f;
		glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 0, shc.x, shc.y, shc.z, 1);

		glDrawElements(GL_TRIANGLE_STRIP, striplen, GL_UNSIGNED_SHORT, strip);

		terrainShaders[header->nLayers - 1]->unbind();

	}
	else
	{
		// FIXED-FUNCTION

		// first pass: base texture
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);

		textures[0]->Bind();

		glActiveTexture(GL_TEXTURE1);
		glDisable(GL_TEXTURE_2D);

		glDisable(GL_BLEND);
		drawPass(animated[0]);
		glEnable(GL_BLEND);

		if (header->nLayers > 1)
		{
			//glDepthFunc(GL_EQUAL); // GL_LEQUAL is fine too...?
			glDepthMask(GL_FALSE);
		}

		// Layers
		for (uint32_t i = 1; i < header->nLayers; i++)
		{
			// Common texture
			textures[i]->Bind();
			glEnable(GL_TEXTURE_2D);

			// this time, use blending:
			alphamaps[i - 1]->Bind(1);
			glEnable(GL_TEXTURE_2D);

			drawPass(animated[i]);

		}

		if (header->nLayers > 1)
		{
			glDepthMask(GL_TRUE);
		}

		// shadow map
		if (header->flags & MCNK_Header::FLAG_HAS_MCSH)
		{
			glActiveTexture(GL_TEXTURE0);
			glDisable(GL_TEXTURE_2D);

			vec3 shc = _World->skies->colorSet[SHADOW_COLOR] * 0.3f;
			glColor4f(shc.x, shc.y, shc.z, 1);

			glActiveTexture(GL_TEXTURE1);
			shadow->Bind(1);
			glEnable(GL_TEXTURE_2D);

			drawPass(0);

			glColor4f(1, 1, 1, 1);
		}
	}
}

void MapChunk::drawNoDetail()
{
	glActiveTexture(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glColor3fv(&_World->skies->colorSet[FOG_COLOR].x);

	// low detail version
	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glDisableClientState(GL_NORMAL_ARRAY);
	glDrawElements(GL_TRIANGLE_STRIP, stripsize, GL_UNSIGNED_SHORT, _World->mapstrip);
	glEnableClientState(GL_NORMAL_ARRAY);

	glColor4f(1, 1, 1, 1);

	glEnable(GL_LIGHTING);
	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
}

