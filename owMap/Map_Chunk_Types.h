#pragma once

struct MCNK_Header
{
	struct
	{
		uint32_t has_mcsh : 1;
		uint32_t impass : 1;
		uint32_t lq_river : 1;
		uint32_t lq_ocean : 1;
		uint32_t lq_magma : 1;
		uint32_t lq_slime : 1;
		uint32_t has_mccv : 1;
		uint32_t unknown_0x80 : 1;
		uint32_t : 7;
				   uint32_t do_not_fix_alpha_map : 1;      // "fix" alpha maps in MCAL (4 bit alpha maps are 63*63 instead of 64*64). Note that this also means that it *has* to be 4 bit alpha maps, otherwise UnpackAlphaShadowBits will assert.
				   uint32_t : 15;
	} flags;

	uint32_t indexX;
	uint32_t indexY;

	uint32_t nLayers;
	uint32_t nDoodadRefs;

	uint32_t ofsHeight;
	uint32_t ofsNormal;
	uint32_t ofsLayer;
	uint32_t ofsRefs;

	uint32_t ofsAlpha;
	uint32_t sizeAlpha;

	uint32_t ofsShadow;
	uint32_t sizeShadow;

	uint32_t areaid;
	uint32_t nMapObjRefs;

	uint32_t holes;
	uint16_t s1; // UINT2[8][8] ReallyLowQualityTextureingMap;	// the content is the layer being on top, I guess.
	uint16_t s2;
	uint32_t d1;
	uint32_t d2;
	uint32_t d3;
	uint32_t predTex;

	uint32_t nEffectDoodad;

	uint32_t ofsSndEmitters;
	uint32_t nSndEmitters;

	uint32_t ofsLiquid;
	uint32_t sizeLiquid;

	float zpos;
	float xpos;
	float ypos;

	uint32_t ofsMCCV;            // only with flags.has_mccv, had uint32_t textureId; in ObscuR's structure.
	uint32_t ofsMCLV;

	uint32_t effectId;
};

//

struct MH2O_Header
{
	uint32_t offsetInstances; // This is an Offset to the first Water Layer, it has tobe an offset because there can be multiple layers.
	uint32_t layersCount; // 0 if the chunk has no liquids. If > 1, the offsets will point to arrays.

						  /*
						  This field requires some explanation.
						  Its an offset to a uint8_t[8] data block.
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
	uint32_t offsetAttributes;
};

struct MH2O_WaterLayer
{
	uint16_t flags;
	uint16_t type;

	float levels[2];

	uint8_t x;
	uint8_t y;
	uint8_t w;
	uint8_t h;

	bool hasmask;
	uint8_t mask[8];

	vector<float> heights;
	vector<uint8_t> depths;
	vector<bool> renderTiles;
};

struct MH2O_Vertex
{
	MH2O_Vertex(vec3 _position, vec2 _textureCoord, vec3 _normal) :
		position(_position),
		textureCoord(_textureCoord),
		normal(_normal)
	{}

	vec3 position;
	vec2 textureCoord;
	vec3 normal;
};

//

enum load_phases
{
	main_file,
	tex,
	obj
};