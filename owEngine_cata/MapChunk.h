#pragma once

#include "WowTypes.h"

class Liquid;

struct MCNK_Header
{
	/*struct {
		uint32_t has_mcsh : 1;
		uint32_t impass : 1;
		uint32_t lq_river : 1;
		uint32_t lq_ocean : 1;
		uint32_t lq_magma : 1;
		uint32_t lq_slime : 1;
		uint32_t has_mccv : 1;
		uint32_t unknown_0x80 : 1;
		uint32_t : 7;
		uint32_t do_not_fix_alpha_map : 1;      // "fix" alpha maps in MCAL (4 bit alpha maps are 63*63 instead of 64*64).
												// Note that this also means that it *has* to be 4 bit alpha maps, otherwise UnpackAlphaShadowBits will assert.
		uint32_t : 15;
	} flags;*/

	enum
	{
		FLAG_HAS_MCSH = 0x01,
		FLAG_IMPASS = 0x02,
		FLAG_LQ_RIVER = 0x04,
		FLAG_LQ_OCEAN = 0x08,
		FLAG_LQ_MAGMA = 0x10,
		FLAG_LQ_SLIME = 0x20,
		FLAG_HAS_MCCV = 0x40,
		FLAG_DO_NOT_FIX_ALPHAS = 0x8000,
	};

	uint32_t flags;
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

enum load_phases
{
	main_file,
	tex,
	obj
};

class MapChunk
{
private:
	MCNK_Header* header;
	bool MCLV_exists;
	bool colorBufferEnable;

public:
	float m_GamePositionX, ybase, m_GamePositionZ;
	float r;

	int areaID;

	bool haswater;

	bool visible;
	bool hasholes;
	float waterlevel[2];

	Texture* textures[4];
	Texture* alphamaps[3];
	Texture* shadow;
	Texture* blend;

	int animated[4];

	GLuint vertices, normals, colorBuffer;

	GLuint vertexBuffer;
	GLuint indicesBuffer;
	GLuint normalsBuffer;

	short *strip;
	int striplen;

	Liquid* lq;

	vec3 vmin, vmax, vcenter;

	MapChunk() : m_GamePositionX(0), ybase(0), m_GamePositionZ(0), r(0), areaID(-1),
		haswater(false), visible(false), hasholes(false), shadow(0), blend(0),
		vertices(0), normals(0), strip(0), striplen(0), lq(0)
	{

		waterlevel[0] = 0;
		waterlevel[1] = 0;

		shadow = _TexturesMgr->Generate();

		blend = _TexturesMgr->Generate();

		for (int i = 0; i < 3; i++)
			alphamaps[i] = _TexturesMgr->Generate();

		colorBufferEnable = false;
	}

	void init(vector<Texture*>* mt, File &f, load_phases _phase);
	void destroy();
	void initStrip(int holes);

	void draw();
	void drawNoDetail();
	void drawPass(int anim);
	//void drawWater();

};