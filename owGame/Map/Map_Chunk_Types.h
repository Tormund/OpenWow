#pragma once

struct MCNK_Header
{
	struct
	{
		uint32 has_mcsh : 1;
		uint32 impass : 1;
		uint32 lq_river : 1;
		uint32 lq_ocean : 1;
		uint32 lq_magma : 1;
		uint32 lq_slime : 1;
		uint32 has_mccv : 1;
		uint32 unknown_0x80 : 1;
		uint32 : 7;
				   uint32 do_not_fix_alpha_map : 1;      // "fix" alpha maps in MCAL (4 bit alpha maps are 63*63 instead of 64*64). Note that this also means that it *has* to be 4 bit alpha maps, otherwise UnpackAlphaShadowBits will assert.
				   uint32 : 15;
	} flags;

	uint32 indexX;
	uint32 indexY;

	uint32 nLayers;
	uint32 nDoodadRefs;

	uint32 ofsHeight;
	uint32 ofsNormal;
	uint32 ofsLayer;
	uint32 ofsRefs;

	uint32 ofsAlpha;
	uint32 sizeAlpha;

	uint32 ofsShadow;
	uint32 sizeShadow;

	uint32 areaid;
	uint32 nMapObjRefs;

	uint32 holes;
	uint16 s1; // UINT2[8][8] ReallyLowQualityTextureingMap;	// the content is the layer being on top, I guess.
	uint16 s2;
	uint32 d1;
	uint32 d2;
	uint32 d3;
	uint32 predTex;

	uint32 nEffectDoodad;

	uint32 ofsSndEmitters;
	uint32 nSndEmitters;

	uint32 ofsLiquid;
	uint32 sizeLiquid;

	float zpos;
	float xpos;
	float ypos;

	uint32 ofsMCCV;            // only with flags.has_mccv, had uint32 textureId; in ObscuR's structure.
	uint32 ofsMCLV;

	uint32 effectId;
};

//

struct MH2O_Header
{
	uint32 offsetInstances; // This is an Offset to the first Water Layer, it has tobe an offset because there can be multiple layers.
	uint32 layersCount; // 0 if the chunk has no liquids. If > 1, the offsets will point to arrays.

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
	uint16 flags;
	uint16 type;

	float levels[2];

	uint8 x;
	uint8 y;
	uint8 w;
	uint8 h;

	bool hasmask;
	uint8 mask[8];

	vector<float> heights;
	vector<uint8> depths;
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