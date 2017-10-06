#include "../stdafx.h"

// Include
#include "Map_Tile.h"

// General
#include "Map_Chunk.h"

// Additional
#include "../Environment/EnvironmentManager.h"

struct MH20_Attributes
{
	uint64_t fishable;               // seems to be usable as visibility information.
	uint64_t deep;
};

struct MH2O_Instance
{
	/*
	uint16 flags;
	known values:
	2
	-> Means no min and max values
	-> type == 2 ?
	-> ocean

	5
	-> Means min and max values are given
	-> type == 0 ?
	-> waterfalls, rivers, lakes (everything that is not on ocean level)

	[TODO] Find all other values
	*/
	uint16 liquidType;

	/*
	uint16 type;
	The type seems tobee depend on the flags ??
	Maybe its an uint32 and in general -> flags

	[TODO] Find all other values
	*/
	uint16 liquidObject;

	/*
	float min;
	The minimum height from all vertices if flags == 5
	*/
	float minHeightLevel;

	/*
	float max;
	The maximum height from all vertices if flags == 5
	*/
	float maxHeightLevel;

	/*
	uint8 x, y, w, h;
	These 4 values are used to define the size of the sub mask and
	the count of height and alpha values needed.

	x and y can be 0-7
	w and h can be 1-8

	Lets say we have a VisibilityMask like this:
	00000000
	00000000
	00000000
	00000000
	00111000
	11111100
	11111111
	11111111

	And the given Values for x y w h are 0 4 8 4 it means there MAYBE more info for this part (marked with X):
	00000000
	00000000
	xy 00000000
	\00000000_
	XXXXXXXX|
	XXXXXXXX| h
	XXXXXXXX|
	XXXXXXXX_
	|---w--|
	*/
	uint8 xOffset;
	uint8 yOffset;
	uint8 width;
	uint8 height;

	/*
	This is a more detailed version of the VisibilityMask. Its an uint8[w*h/8] data block.
	The grid is created as before but every line only contains w bits
	If the offset is 0 then every quad marked by x y w h is displayed
	*/
	uint32 offsetExistsBitmap;


	/*
	uint32 ofsHeigthAlpha;

	This offset points to an array of heights and after that there is an array of alpha values.

	the size of both arrays is (w+1)*(h+1)
	8*8 quads -> 9*9 vertices so if w or h is 8 we need 9 values this explains the "+1"

	the heights array float[(w+1)*(h+1)] is only present if the flags == 5 otherwise (2) its not required

	the alpha array uint8[(w+1)*(h+1)] seems tobe always present if the offset is given and comes always
	after the heights array and if heights are not given it is directly at ofsHeigthAlpha
	*/
	uint32 offsetVertexData;
};

//

void MapChunk::TryInitMH2O(MH2O_Header* _header, File& f)
{
	//Debug::Green("MH2O: layers = %d", mh2o_Header->layersCount);

	for (uint32 j = 0; j < _header->layersCount; j++)
	{
		MH2O_Instance* mh2o_instance = new MH2O_Instance;
		mh2o_instance = (MH2O_Instance*)(f.GetDataFromCurrent() + _header->offsetInstances + sizeof(MH2O_Instance) * j);

		//auto LTYPE = DBC_LiquidType.getByID(mh2o_instance->liquidType);
		//auto VERTEX_FMT = LTYPE->Get_LiquidMaterialID();

		DBC_LiquidTypeRecord* LTYPE = DBC_LiquidType.getByID(mh2o_instance->liquidType);
		//Debug::Error("TEst id = %s", LTYPE->Get_Name());
		auto VERTEX_FMT = LTYPE->Get_LiquidMaterialID();

		if (mh2o_instance->minHeightLevel - mh2o_instance->maxHeightLevel > 0.001f)
		{
			Debug::Green("MinHeight %f:", mh2o_instance->minHeightLevel);
			Debug::Green("MaxHeight %f:", mh2o_instance->maxHeightLevel);
			Debug::Error("MIN WATER != MAX_WATER!!!!");
		}

		//Debug::Green("---------");
		//Debug::Green("Chunk [%d][%d]", i / C_ChunksInTile, i % C_ChunksInTile);
		//Debug::Green("Layer %d:", j);

		//Debug::Green("MATERIAL = %d", LTYPE->Get_LiquidMaterialID());
		//Debug::Green("VERTEX FMT = %d", VERTEX_FMT->Get_LiquidVertexFormat());

		MH2O_WaterLayer waterLayer;

		waterLayer.x = mh2o_instance->xOffset;
		waterLayer.y = mh2o_instance->yOffset;
		waterLayer.w = mh2o_instance->width;
		waterLayer.h = mh2o_instance->height;
		waterLayer.flags = mh2o_instance->liquidType;
		waterLayer.levels[0] = mh2o_instance->minHeightLevel;
		waterLayer.levels[1] = mh2o_instance->maxHeightLevel;
		waterLayer.type = mh2o_instance->liquidObject;

		waterLayer.hasmask = mh2o_instance->offsetExistsBitmap != 0;
		if (waterLayer.hasmask)
		{
			unsigned co = mh2o_instance->width * mh2o_instance->height / 8;

			if (mh2o_instance->width * mh2o_instance->height % 8 != 0)
				co++;

			memcpy(waterLayer.mask, f.GetDataFromCurrent() + mh2o_instance->offsetExistsBitmap, co);

			for (size_t k = 0; k < (size_t)(waterLayer.w * waterLayer.h); k++)
			{
				if (getBitL2H(waterLayer.mask, (uint32)k))
				{
					waterLayer.renderTiles.push_back(true);
				}
				else
				{
					waterLayer.renderTiles.push_back(false);
				}
			}
		}

		// Check exists vertex data
		if (mh2o_instance->offsetVertexData == 0)
		{
			//Debug::Error("Liquid instance NOT offset Vertex data!!!");
			continue;
		}

		//Debug::Error("GOOOOD!!!");

		const uint32 vertexDataSize = (mh2o_instance->width + 1) * (mh2o_instance->height + 1);

		if (VERTEX_FMT->Get_LiquidVertexFormat() == 0)
		{
			//Debug::Info("Case 0, Height and Depth data");

			float* pHeights = (float*)(f.GetDataFromCurrent() + mh2o_instance->offsetVertexData);
			uint8* pDepths = (uint8*)f.GetDataFromCurrent() + mh2o_instance->offsetVertexData + (sizeof(float) * vertexDataSize);

			for (uint32 g = 0; g < vertexDataSize; g++)
			{
				waterLayer.heights.push_back(mh2o_instance->maxHeightLevel);
				//waterLayer.heights.push_back(pHeights[g]);
				waterLayer.depths.push_back(pDepths[g]);
				//Debug::Info("Height [%f]", pHeights[g]);
				//Debug::Info("Depth [%d]", pDepths[g]);
			}
		}
		else if (VERTEX_FMT->Get_LiquidVertexFormat() == 1)
		{
			//Debug::Info("Case 1, Height and Texture Coordinate data");

			struct uv_map_entry
			{
				uint16 x;                      // divided by 8 for shaders
				uint16 y;
			};

			float* pHeights = (float*)(f.GetDataFromCurrent() + mh2o_instance->offsetVertexData);
			uv_map_entry* pUVMap = (uv_map_entry*)f.GetDataFromCurrent() + mh2o_instance->offsetVertexData + (sizeof(float) * vertexDataSize);

			for (uint32 g = 0; g < vertexDataSize; g++)
			{
				waterLayer.heights.push_back(pHeights[g]);
				//waterLayer.alphas.push_back( pUnknowns[g] );
				//Debug::Info("Height [%f]", pHeights[g]);
				//Debug::Info("Textures [%d] Y[%d]", pUVMap[g].x, pUVMap[g].y);
			}
		}
		else
		{
			Debug::Error("Case %d : UNKNOWN!!!", mh2o_instance->liquidObject);
		}

		// Add water

		m_WaterLayers.push_back(waterLayer);
	}
}

void MapChunk::createBuffer()
{
	const float defaultTextureCoord = 1.0f;
	const vec3 defaultNormal = vec3(0.0f, 1.0f, 0.0f);

	vector<MH2O_Vertex> mh2oVertices;

	for (unsigned l = 0; l < m_WaterLayers.size(); l++)
	{
		MH2O_WaterLayer& layer = m_WaterLayers[l];

		for (uint8 y = layer.y; y < layer.h + layer.y; y++)
		{
			for (uint8 x = layer.x; x < layer.w + layer.x; x++)
			{

				unsigned tx = x - layer.x;
				unsigned ty = y - layer.y;

				// p1--p4
				// |    |  // this is GL_QUADS 
				// p2--p3
				unsigned p1 = tx + ty           * (layer.w + 1);
				unsigned p2 = tx + (ty + 1)     * (layer.w + 1);
				unsigned p3 = tx + 1 + (ty + 1) * (layer.w + 1);
				unsigned p4 = tx + 1 + ty       * (layer.w + 1);

				// alpha values helper
				/*float a1, a2, a3, a4;
				a1 = a2 = a3 = a4 = 1.0f;
				if (layer.depths.size() != 0)
				{
				a1 = (float)layer.depths[p1] / 255.f * 1.5f + 0.3f; // whats the magic formular here ???
				a2 = (float)layer.depths[p2] / 255.f * 1.5f + 0.3f;
				a3 = (float)layer.depths[p3] / 255.f * 1.5f + 0.3f;
				a4 = (float)layer.depths[p4] / 255.f * 1.5f + 0.3f;
				}*/

				// height values helper
				float h1, h2, h3, h4;
				h1 = h2 = h3 = h4 = 0.0f;
				if (layer.heights.size() != 0)
				{
					h1 = layer.heights[p1];
					h2 = layer.heights[p2];
					h3 = layer.heights[p3];
					h4 = layer.heights[p4];
				}

				if (layer.renderTiles.size() != 0)
				{
					if (!layer.renderTiles[tx + ty * layer.w])
					{
						continue;
					}
				}

				// Insert vertex

				mh2oVertices.push_back(MH2O_Vertex(
					vec3(m_GamePositionX + C_UnitSize * static_cast<float>(x), h1, m_GamePositionZ + C_UnitSize * static_cast<float>(y)),
					vec2(0.0f, 0.0f),
					defaultNormal
				));

				mh2oVertices.push_back(MH2O_Vertex(
					vec3(m_GamePositionX + C_UnitSize * static_cast<float>(x), h2, m_GamePositionZ + C_UnitSize + C_UnitSize * static_cast<float>(y)),
					vec2(0.0f, defaultTextureCoord),
					defaultNormal
				));

				mh2oVertices.push_back(MH2O_Vertex(
					vec3(m_GamePositionX + C_UnitSize + C_UnitSize * static_cast<float>(x), h3, m_GamePositionZ + C_UnitSize + C_UnitSize * static_cast<float>(y)),
					vec2(defaultTextureCoord, defaultTextureCoord),
					defaultNormal
				));

				mh2oVertices.push_back(MH2O_Vertex(
					vec3(m_GamePositionX + C_UnitSize + C_UnitSize * static_cast<float>(x), h4, m_GamePositionZ + C_UnitSize * static_cast<float>(y)),
					vec2(defaultTextureCoord, 0.0f),
					defaultNormal
				));
			}
		}
	}

	globalBufferSize = static_cast<uint32>(mh2oVertices.size());

	if (globalBufferSize == 0)
	{
		return;
	}

	glGenBuffers(1, &globalBufferWater);
	glBindBuffer(GL_ARRAY_BUFFER, globalBufferWater);

	glBufferData(GL_ARRAY_BUFFER, mh2oVertices.size() * sizeof(MH2O_Vertex), mh2oVertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MapChunk::initTextures(const char* basename, int first, int last)
{
	char buf[256];
	for (int i = first; i <= last; i++)
	{
		sprintf_s(buf, "%s.%d.blp", basename, i);
		wTextures.push_back(_TexturesMgr->Add(buf));
	}
}

void MapChunk::drawWater()
{
	if (wTextures.size() == 0)
		return;

	if (globalBufferSize == 0)
		return;

	glBindBuffer(GL_ARRAY_BUFFER, globalBufferWater);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const GLvoid*)(0));	
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const GLvoid*)(3 * sizeof(float)));	
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const GLvoid*)(5 * sizeof(float)));

	size_t texidx = (size_t)(_EnvironmentManager->animtime / 60.0) % wTextures.size();
	wTextures[texidx]->Bind();
	wTextures[texidx]->Bind(5);

	_TechniquesMgr->m_WMO_MH2O_GeometryPass->SetWaterColor(_EnvironmentManager->GetSkyColor(RIVER_COLOR_LIGHT));
	//_TechniquesMgr->m_WMO_MH2O_GeometryPass->SetWaterColor(vec3(1.0, 1.0, 1.0));

	glDrawArrays(GL_QUADS, 0, globalBufferSize);
	PERF_INC(PERF_MAP_CHUNK_MH20);

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	/*glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
	glEnable(GL_TEXTURE_2D);

	for (unsigned l = 0; l < waterLayer.size(); l++)
	{
	MH2O_WaterLayer& layer = waterLayer[l];

	for (uint8 y = layer.y; y < layer.h + layer.y; y++)
	{
	for (uint8 x = layer.x; x < layer.w + layer.x; x++)
	{

	unsigned tx = x - layer.x;
	unsigned ty = y - layer.y;

	// p1--p4
	// |    |  // this is GL_QUADS
	// p2--p3
	unsigned p1 = tx + ty           * (layer.w + 1);
	unsigned p2 = tx + (ty + 1)     * (layer.w + 1);
	unsigned p3 = tx + 1 + (ty + 1) * (layer.w + 1);
	unsigned p4 = tx + 1 + ty       * (layer.w + 1);

	// alpha values helper
	float a1, a2, a3, a4;
	a1 = a2 = a3 = a4 = 1.0f;
	if (layer.depths.size() != 0)
	{
	a1 = (float)layer.depths[p1] / 255.f * 1.5f + 0.3f; // whats the magic formular here ???
	a2 = (float)layer.depths[p2] / 255.f * 1.5f + 0.3f;
	a3 = (float)layer.depths[p3] / 255.f * 1.5f + 0.3f;
	a4 = (float)layer.depths[p4] / 255.f * 1.5f + 0.3f;
	}

	// height values helper
	float h1, h2, h3, h4;
	h1 = h2 = h3 = h4 = 0.0f;
	if (layer.heights.size() != 0)
	{
	h1 = layer.heights[p1];
	h2 = layer.heights[p2];
	h3 = layer.heights[p3];
	h4 = layer.heights[p4];
	}

	if (layer.renderTiles.size() != 0)
	{
	if (!layer.renderTiles[tx + ty * layer.w])
	{
	continue;
	}
	}

	glColor4f(1.0, 1.0, 1.0, 1.0);

	glBegin(GL_QUADS);
	{
	glTexCoord2f(0, 0);
	glNormal3f(0, 1, 0);
	glVertex3f(m_GamePositionX + C_UnitSize * x, h1, m_GamePositionZ + C_UnitSize * y);


	glTexCoord2f(0, defaultTextureCoord);
	glNormal3f(0, 1, 0);
	glVertex3f(m_GamePositionX + C_UnitSize * x, h2, m_GamePositionZ + C_UnitSize + C_UnitSize * y);


	glTexCoord2f(defaultTextureCoord, defaultTextureCoord);
	glNormal3f(0, 1, 0);
	glVertex3f(m_GamePositionX + C_UnitSize + C_UnitSize * x, h3, m_GamePositionZ + C_UnitSize + C_UnitSize * y);

	glTexCoord2f(defaultTextureCoord, 0);
	glNormal3f(0, 1, 0);
	glVertex3f(m_GamePositionX + C_UnitSize + C_UnitSize * x, h4, m_GamePositionZ + C_UnitSize * y);
	}
	glEnd();
	}
	}
	}

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);*/
}