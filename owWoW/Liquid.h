#pragma once


enum MCNK_MCLQ_LiquidType
{
	lq_river,
	lq_ocean,
	lq_magma,
	lq_slime
};

#include "../shared/pack_begin.h"

struct Liquid_Vertex
{
	union
	{
		struct SWVert
		{
			char depth;
			char flow0Pct;
			char flow1Pct;
			char filler;
			float height;
		} waterVert;

		struct SOVert
		{
			char depth;
			char foam;
			char wet;
			char filler;
		} oceanVert;

		struct SMOMagmaVert
		{
			uint16 s;
			uint16 t;
			float height;
		} magmaVert;
	};
};

struct Liquid_Flag
{
	uint8 liquid : 6;
	uint8 fishable : 1;
	uint8 shared : 1;

	// 0x01
	// 0x02
	// 0x04
	// 0x08
	// 0x10
	// 0x20

	// 0x40
	// 0x80
};

struct MH2O_Header
{
	uint32 offsetInstances; // This is an Offset to the first Water Layer, it has tobe an offset because there can be multiple layers.
	uint32 layersCount;    // 0 if the chunk has no liquids. If > 1, the offsets will point to arrays.

						/*
						This field requires some explanation.
						Its an offset to a uint8[8] data block.
						the data block contains a bit field 8x8 = 64 bit every bit represents a quad between 4 verts.
						But the bits in this field are only set to 1 if the field is really visible this is not ment in a
						technical way. To make it clear all of the 4 vertices have an alpha value (i explain them later)
						if only one of these alpha values smaller then 10 the bit is not set

						if the first byte is 0x17h which is in binary 00010111b it has to look like this in a grid:

						0123...
						1+-----> bits
						2| 11101000
						3|
						.|
						.V
						bytes

						*/
	uint32 offsetAttributes;
};

struct MH2O_WaterLayer
{
	uint16 LiquidType;
	uint16 LiquidObjectOrLVF;

	float MinHeightLevel;
	float MaxHeightLevel;

	uint8 x;
	uint8 y;
	uint8 Width;
	uint8 Height;

	bool hasmask;
	uint8 mask[8];

	vector<float> heights;
	vector<uint8> depths;
	vector<std::pair<float, float>> textureCoords;

	vector<bool> renderTiles;
};

#include "../shared/pack_end.h"

//

struct LiquidOwner
{
	enum List
	{
		LIQUID_OWNER_WMO,
		LIQUID_OWNER_MAP_CHUNK_MH2O
	};
};

// Includes and using

#include "DBC__Storage.h"
class WMOMaterial;

// Class

class Liquid
{
public:
	Liquid(uint32 x, uint32 y, vec3 base);
	~Liquid();

	void initFromTerrainMH2O(File& f, MH2O_Header* _header);
	void initFromWMO2(File& f, WMOMaterial* _material, const DBC_LiquidTypeRecord* _liquidType, bool _indoor);
	void createBuffer(cvec3 _position);

	void draw();

private:
	void initGeometry(File& f);
	void InitTextures(const DBC_LiquidTypeRecord* _liquidType);

private:
	uint32 m_TilesX, m_TilesY;
	uint32 m_TilesCount;
	vec3 m_Position;

	vec3 m_WaterColorLight;
	vec3 m_WaterColorDark;

	uint32 __geom;
	uint32 globalBufferSize;


	vector<Texture*> textures;


	vector<MH2O_WaterLayer> m_WaterLayers;

	float ydir;
	float texRepeats;

	int type;



	bool trans;

	int shader;

private:
	const vec3 defaultNormal = vec3(0.0f, 1.0f, 0.0f);
};
