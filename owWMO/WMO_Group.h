#pragma once

class WMO;
class Liquid;

struct WMOGroupInfoDef
{
	struct WMOGroupInfoFlags
	{
		uint32 FLAG_HAS_COLLISION : 1;                  // = 0x1			//Has MOBN and MOBR chunk.
		uint32 FLAG_HAS_LIGHTMAP : 1;                   // = 0x2			//Has light map MOLM, MOLD
		uint32 FLAG_HAS_VERTEX_COLORS : 1;              // = 0x4			//Has vertex colors(MOCV chunk).
		uint32 FLAG_IS_OUTDOOR : 1;                     // = 0x8			//EXTERIOR -- Outdoor

		uint32 FLAG_UNK_0x10 : 1;                       // = 0x10			(UNUSED: 20740)
		uint32 FLAG_UNK_0x20 : 1;                       // = 0x20			(UNUSED: 20740)
		uint32 FLAG_DO_NOT_USE_LIGHTING_DIFFUSE : 1;    // = 0x40			//Do not use local diffuse lightning .Applicable for both doodads from this wmo group(color from MODD) and water(CWorldView::GatherMapObjDefGroupLiquids).
		uint32 FLAG_IS_UNREACHABLE : 1;                 // = 0x80			//UNREACHABLE

		uint32 FLAG_UNK_0x100 : 1;                      // = 0x100
		uint32 FLAG_HAS_LIGHTS : 1;                     // = 0x200		//Has m_Lights (MOLR chunk)
		uint32 FLAG_UNK_0x400 : 1;                      // = 0x400
		uint32 FLAG_HAS_DOODADS : 1;                    // = 0x800		//Has doodads(MODR chunk)

		uint32 FLAG_HAS_WATER : 1;                      // = 0x1000		//LIQUIDSURFACE -- Has water(MLIQ chunk)
		uint32 FLAG_IS_INDOOR : 1;                      // = 0x2000		//INTERIOR -- Indoor
		uint32 FLAG_UNK_0x4000 : 1;                     // = 0x4000
		uint32 FLAG_UNK_0x8000 : 1;                     // = 0x8000

		uint32 FLAG_ALWAYS_DRAW : 1;                    // = 0x10000		//ALWAYSDRAW -- clear 0x8 after CMapObjGroup::Create() in MOGP and MOGI
		uint32 FLAG_UNK_0x20000 : 1;                    // = 0x20000		(UNUSED: 20740) Has MORI and MORB chunks.
		uint32 FLAG_HAS_SKYBOX : 1;                     // = 0x40000		//Show m_Skybox -- automatically unset if MOSB not present.
		uint32 FLAG_UNK_0x80000 : 1;                    // = 0x80000		is_not_water_but_ocean, LiquidType related, see below in the MLIQ chunk.

		uint32 FLAG_HAS_2_VERTEX_COLORS_BUFFERS : 1;    // = 0x1000000	//CVERTS2: Has two MOCV chunks : Just add two or don't set 0x4 to only use cverts2.
		uint32 FLAG_HAS_2_TEXTURES_COORDS : 1;          // = 0x2000000	//TVERTS2 : Has two MOTV chunks : Just add two.
		uint32 FLAG_UNK_0x4000000 : 1;                  // = 0x4000000
		uint32 FLAG_UNK_0x8000000 : 1;                  // = 0x8000000

		uint32 FLAG_UNK_0x10000000 : 1;                 // = 0x10000000	(UNUSED: 20740)
		uint32 FLAG_UNK_0x20000000 : 1;				  // = 0x20000000	>> 20740 ᵘ EXTERIOR_CULL
		uint32 FLAG_HAS_3_TEXTURES_COORDS : 1;          // = 0x40000000	//TVERTS3 : Has three MOTV chunks, eg. for MOMT with shader 18.
		uint32 FLAG_UNK_0x80000000 : 1;                 // = 0x80000000	(UNUSED: 20740)

		uint32 FLAG_UNK_0x100000000 : 1;                // = 0x100000000
		uint32 FLAG_UNK_0x200000000 : 1;				  // = 0x200000000 
		uint32 FLAG_UNK_0x400000000 : 1;                // = 0x400000000
		uint32 FLAG_UNK_0x800000000 : 1;                // = 0x800000000
	} flags;

	CAaBox bounding_box;
	int32 nameoffset;                                   // name in MOGN chunk (-1 for no name)

	//
	static const uint32 __size = 32;
};

struct WMOGroupHeader
{
	uint32 groupName;
	uint32 descriptiveGroupName;
	uint32 flags;
	CAaBox boundingBox;

	uint16 portalStart; // Index into the MOPR chunk
	uint16 portalCount; // Number of items used from the MOPR chunk

	// Batches
	uint16 transBatchCount;
	uint16 intBatchCount;
	uint16 extBatchCount;
	uint16 padding_or_batch_type_d; // probably padding, but might be data?

	uint8 m_Fogs[4]; // Up to four indices into the WMO fog list
	uint32 liquidType; // LiquidType related, see below in the MLIQ chunk.

	uint32 wmoGroupId; // WMO group ID (column 4 in WMOAreaTable.dbc)

	enum
	{
		flag2_CanCutTerrain = 1,        // ≥ Mists has portal planes to cut
	};
	uint32 flags2; // Always 0?
	uint32 unk; // Always 0?

	//
	static const uint32 __size = 68;
};

struct WMOGroup_MaterialInfo
{
	struct
	{
		uint8 F_UNK_0x01 : 1;
		uint8 F_NOCAMCOLLIDE : 1;
		uint8 F_DETAIL : 1;
		uint8 F_COLLISION : 1; // Turns off rendering of water ripple effects. May also do more. Should be used for ghost material triangles.
		uint8 F_HINT : 1;
		uint8 F_RENDER : 1;
		uint8 F_UNK_0x40 : 1;
		uint8 F_COLLIDE_HIT : 1;

		bool isTransFace() { return F_UNK_0x01 && (F_DETAIL || F_RENDER); }
		bool isColor() { return !F_COLLISION; }
		bool isRenderFace() { return F_RENDER && !F_DETAIL; }
		bool isCollidable() { return F_COLLISION || isRenderFace(); }
	} flags;

	uint8 materialId;

	//
	static const uint32 __size = 2;
};

struct WMOBatch
{
	int16 bx, by, bz;                      // a bounding box for culling, see "unknown_box" below
	int16 tx, ty, tz;

	uint32 indexStart;
	uint16 indexCount;

	uint16 vertexStart;
	uint16 vertexEnd;

	uint8 flags;
	uint8 material_id; // index in MOMT

	//
	static const uint32 __size = 24;
};

struct WMOLiquidHeader
{
	uint32 X;
	uint32 Y;
	uint32 A;
	uint32 B;
	vec3 pos;
	uint16 type;

	//
	static const uint32 __size = 30;
};

//---

class WMOGroup
{
public:
	WMOGroup(const WMO* _parentWMO, const uint32 m_GroupIndex, File& f);
	~WMOGroup();

	void initDisplayList();
	void initLighting();

	bool draw2();
	bool drawDoodads(uint32 _doodadSet);
	bool drawLiquid();

	void setupFog();


public:
	const WMO* m_ParentWMO;
	const uint32 m_GroupIndex;
	string m_GroupName;

	WMOGroupInfoDef groupInfo;
	WMOGroupHeader m_Header;


public:
	int fog;
	bool visible;
	bool m_EnableOutdoorLights;
	//vector< pair<GLuint, int> > lists;
	GLuint globalBuffer;
	BoundingBox2 bounds;

public:
	//-- Triangles --//
	uint32 nTriangles;
	WMOGroup_MaterialInfo* materials; // MOPY chunk

	unsigned short* indices;          // MOVI chunk

	uint32 nVertices;
	vec3* vertices;                   // MOVT chunk

	vec3* normals;                    // MONR chunk
	vec2* texcoords;                  // MOTV chunk


	//-- Render bathes --//
	uint32 nBatches;
	WMOBatch* m_WMOBatchIndexes;      // MOBA chunk


	//-- Lights --//
	uint32 nLights;
	uint16* m_WMOLightsIndexes;     // MOLR chunk


	//-- Doodads references --//
	uint32 nDoodads;
	uint16* m_DoodadsIndexes;       // MODR chunk


	//-- Collision --//
	// MOBN chunk
	// MOBR chunk

	//-- Vertex colors --//
	bool m_HasVertexColors;
	uint32* m_VertexColors;         // MOCV chunk

	//-- Liquid --//
	Liquid* lq;
};