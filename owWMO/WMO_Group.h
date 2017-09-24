#pragma once

class WMO;
class Liquid;

struct WMOGroupInfoDef
{
	struct WMOGroupInfoFlags
	{
		uint32_t FLAG_HAS_COLLISION : 1;                  // = 0x1			//Has MOBN and MOBR chunk.
		uint32_t FLAG_HAS_LIGHTMAP : 1;                   // = 0x2			//Has light map MOLM, MOLD
		uint32_t FLAG_HAS_VERTEX_COLORS : 1;              // = 0x4			//Has vertex colors(MOCV chunk).
		uint32_t FLAG_IS_OUTDOOR : 1;                     // = 0x8			//EXTERIOR -- Outdoor

		uint32_t FLAG_UNK_0x10 : 1;                       // = 0x10			(UNUSED: 20740)
		uint32_t FLAG_UNK_0x20 : 1;                       // = 0x20			(UNUSED: 20740)
		uint32_t FLAG_DO_NOT_USE_LIGHTING_DIFFUSE : 1;    // = 0x40			//Do not use local diffuse lightning .Applicable for both doodads from this wmo group(color from MODD) and water(CWorldView::GatherMapObjDefGroupLiquids).
		uint32_t FLAG_IS_UNREACHABLE : 1;                 // = 0x80			//UNREACHABLE

		uint32_t FLAG_UNK_0x100 : 1;                      // = 0x100
		uint32_t FLAG_HAS_LIGHTS : 1;                     // = 0x200		//Has lights (MOLR chunk)
		uint32_t FLAG_UNK_0x400 : 1;                      // = 0x400
		uint32_t FLAG_HAS_DOODADS : 1;                    // = 0x800		//Has doodads(MODR chunk)

		uint32_t FLAG_HAS_WATER : 1;                      // = 0x1000		//LIQUIDSURFACE -- Has water(MLIQ chunk)
		uint32_t FLAG_IS_INDOOR : 1;                      // = 0x2000		//INTERIOR -- Indoor
		uint32_t FLAG_UNK_0x4000 : 1;                     // = 0x4000
		uint32_t FLAG_UNK_0x8000 : 1;                     // = 0x8000

		uint32_t FLAG_ALWAYS_DRAW : 1;                    // = 0x10000		//ALWAYSDRAW -- clear 0x8 after CMapObjGroup::Create() in MOGP and MOGI
		uint32_t FLAG_UNK_0x20000 : 1;                    // = 0x20000		(UNUSED: 20740) Has MORI and MORB chunks.
		uint32_t FLAG_HAS_SKYBOX : 1;                     // = 0x40000		//Show skybox -- automatically unset if MOSB not present.
		uint32_t FLAG_UNK_0x80000 : 1;                    // = 0x80000		is_not_water_but_ocean, LiquidType related, see below in the MLIQ chunk.

		uint32_t FLAG_HAS_2_VERTEX_COLORS_BUFFERS : 1;    // = 0x1000000	//CVERTS2: Has two MOCV chunks : Just add two or don't set 0x4 to only use cverts2.
		uint32_t FLAG_HAS_2_TEXTURES_COORDS : 1;          // = 0x2000000	//TVERTS2 : Has two MOTV chunks : Just add two.
		uint32_t FLAG_UNK_0x4000000 : 1;                  // = 0x4000000
		uint32_t FLAG_UNK_0x8000000 : 1;                  // = 0x8000000

		uint32_t FLAG_UNK_0x10000000 : 1;                 // = 0x10000000	(UNUSED: 20740)
		uint32_t FLAG_UNK_0x20000000 : 1;				  // = 0x20000000	>> 20740 ᵘ EXTERIOR_CULL
		uint32_t FLAG_HAS_3_TEXTURES_COORDS : 1;          // = 0x40000000	//TVERTS3 : Has three MOTV chunks, eg. for MOMT with shader 18.
		uint32_t FLAG_UNK_0x80000000 : 1;                 // = 0x80000000	(UNUSED: 20740)

		uint32_t FLAG_UNK_0x100000000 : 1;                // = 0x100000000
		uint32_t FLAG_UNK_0x200000000 : 1;				  // = 0x200000000 
		uint32_t FLAG_UNK_0x400000000 : 1;                // = 0x400000000
		uint32_t FLAG_UNK_0x800000000 : 1;                // = 0x800000000
	} flags;

	CAaBox bounding_box;
	int32_t nameoffset;                                   // name in MOGN chunk (-1 for no name)

	//
	static const size_t __size = 32;
};

struct WMOGroupHeader
{
	uint32_t groupName;
	uint32_t descriptiveGroupName;
	uint32_t flags;
	CAaBox boundingBox;

	uint16_t portalStart; // Index into the MOPR chunk
	uint16_t portalCount; // Number of items used from the MOPR chunk

	// Batches
	uint16_t transBatchCount;
	uint16_t intBatchCount;
	uint16_t extBatchCount;
	uint16_t padding_or_batch_type_d; // probably padding, but might be data?

	uint8_t fogs[4]; // Up to four indices into the WMO fog list
	uint32_t liquidType; // LiquidType related, see below in the MLIQ chunk.

	uint32_t wmoGroupId; // WMO group ID (column 4 in WMOAreaTable.dbc)

	enum
	{
		flag2_CanCutTerrain = 1,        // ≥ Mists has portal planes to cut
	};
	uint32_t flags2; // Always 0?
	uint32_t unk; // Always 0?

	//
	const static size_t __size = 68;
};

struct WMOGroup_MaterialInfo
{
	struct
	{
		uint8_t F_UNK_0x01 : 1;
		uint8_t F_NOCAMCOLLIDE : 1;
		uint8_t F_DETAIL : 1;
		uint8_t F_COLLISION : 1; // Turns off rendering of water ripple effects. May also do more. Should be used for ghost material triangles.
		uint8_t F_HINT : 1;
		uint8_t F_RENDER : 1;
		uint8_t F_UNK_0x40 : 1;
		uint8_t F_COLLIDE_HIT : 1;

		bool isTransFace() { return F_UNK_0x01 && (F_DETAIL || F_RENDER); }
		bool isColor() { return !F_COLLISION; }
		bool isRenderFace() { return F_RENDER && !F_DETAIL; }
		bool isCollidable() { return F_COLLISION || isRenderFace(); }
	} flags;

	uint8_t materialId;

	//
	const static size_t __size = 2;
};

struct WMOBatch
{
	uint32_t color0;
	uint32_t color1;
	uint32_t color2;

	uint32_t indexStart;
	uint16_t indexCount;

	uint16_t vertexStart;
	uint16_t vertexEnd;

	uint8_t flags;
	uint8_t material_id; // index in MOMT

	//
	const static size_t __size = 24;
};

struct WMOLiquidHeader
{
	uint32_t X;
	uint32_t Y;
	uint32_t A;
	uint32_t B;
	vec3 pos;
	uint16_t type;

	//
	const static size_t __size = 30;
};

//---

class WMOGroup
{
public:
	WMOGroup();
	~WMOGroup();

	void init(WMO* wmo, File& f, int num, char* names);
	void initDisplayList();
	void initLighting();

	void draw(cvec3 ofs, const float roll);
	void drawDoodads(int doodadset, cvec3 ofs, const float roll);
	void drawLiquid();

	void setupFog();

	WMO* wmo;

	vec3 center;
	float rad;
	int num;
	int fog;

	vector< pair<GLuint, int> > lists;

public:
	vec3 b1, b2;
	vec3 vmin, vmax;
	bool indoor;
	bool visible;

	bool m_EnableOutdoorLights;
	string name;

private:
	WMOGroupInfoDef groupInfo;


public:
	WMOGroupHeader wmoGroupHeader;

	//-- Triangles --//
	uint32_t nTriangles;
	WMOGroup_MaterialInfo* materials; // MOPY chunk

	unsigned short* indices;          // MOVI chunk

	uint32_t nVertices;
	vec3* vertices;                   // MOVT chunk

	vec3* normals;                    // MONR chunk
	vec2* texcoords;                  // MOTV chunk


	//-- Render bathes --//
	uint32_t nBatches;
	WMOBatch* m_WMOBatchIndexes;      // MOBA chunk


	//-- Lights --//
	uint32_t nLights;
	uint16_t* m_WMOLightsIndexes;     // MOLR chunk


	//-- Doodads references --//
	uint32_t nDoodads;
	uint16_t* m_DoodadsIndexes;       // MODR chunk


	//-- Collision --//
	// MOBN chunk
	// MOBR chunk


	//-- Vertex colors --//
	bool hascv;
	uint32_t* m_VertexColors;         // MOCV chunk

	//-- Liquid --//
	Liquid* lq;
};