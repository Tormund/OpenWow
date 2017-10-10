#pragma once

#include "../Liquid.h"

class WMO;

struct WMOGroupFlags
{
	uint32 FLAG_HAS_COLLISION : 1;                  // = 0x1			//Has MOBN and MOBR chunk.
	uint32 FLAG_UNK_0x2 : 1;                        // = 0x2			// UNUSED
	uint32 FLAG_HAS_VERTEX_COLORS : 1;              // = 0x4			//Has vertex colors(MOCV chunk).
	uint32 FLAG_IS_OUTDOOR : 1;                     // = 0x8			//EXTERIOR -- Outdoor

	uint32 FLAG_UNK_0x10 : 1;                       // = 0x10			(UNUSED: 20740)
	uint32 FLAG_UNK_0x20 : 1;                       // = 0x20			(UNUSED: 20740)
	uint32 FLAG_DO_NOT_USE_LIGHTING_DIFFUSE : 1;    // = 0x40			//Do not use local diffuse lightning .Applicable for both doodads from this wmo group(color from MODD) and water(CWorldView::GatherMapObjDefGroupLiquids).
	uint32 FLAG_IS_UNREACHABLE : 1;                 // = 0x80			//UNREACHABLE

	uint32 FLAG_UNK_0x100 : 1;                      // = 0x100
	uint32 FLAG_HAS_LIGHTS : 1;                     // = 0x200		    // Has m_Lights (MOLR chunk)
	uint32 FLAG_UNK_0x400 : 1;                      // = 0x400
	uint32 FLAG_HAS_DOODADS : 1;                    // = 0x800		    // Has doodads(MODR chunk)

	uint32 FLAG_HAS_WATER : 1;                      // = 0x1000		    //LIQUIDSURFACE -- Has water(MLIQ chunk)
	uint32 FLAG_IS_INDOOR : 1;                      // = 0x2000		    //INTERIOR -- Indoor
	uint32 FLAG_UNK_0x4000 : 1;                     // = 0x4000
	uint32 FLAG_UNK_0x8000 : 1;                     // = 0x8000

	uint32 FLAG_ALWAYS_DRAW : 1;                    // = 0x10000		//ALWAYSDRAW -- clear 0x8 after CMapObjGroup::Create() in MOGP and MOGI
	uint32 FLAG_UNK_0x20000 : 1;                    // = 0x20000		Has MORI and MORB chunks.
	uint32 FLAG_HAS_SKYBOX : 1;                     // = 0x40000		//Show m_Skybox -- automatically unset if MOSB not present.
	uint32 FLAG_UNK_0x80000 : 1;                    // = 0x80000		is_not_water_but_ocean, LiquidType related, see below in the MLIQ chunk.

	uint32 FLAG_UNK_0x100000 : 1;
	uint32 FLAG_UNK_0x200000 : 1;
	uint32 FLAG_UNK_0x400000 : 1;
	uint32 FLAG_UNK_0x800000 : 1;

	uint32 FLAG_HAS_2_MOCV : 1;                     // = 0x1000000	    //Has two MOCV chunks : Just add two or don't set 0x4 to only use cverts2.
	uint32 FLAG_HAS_2_MOTV : 1;                     // = 0x2000000	    //Has two MOTV chunks : Just add two.
	uint32 FLAG_UNK_0x4000000 : 1;                  // = 0x4000000
	uint32 FLAG_UNK_0x8000000 : 1;                  // = 0x8000000

	uint32 FLAG_UNK_0x10000000 : 1;                 // = 0x10000000
	uint32 FLAG_UNK_0x20000000 : 1;				    // = 0x20000000	    EXTERIOR_CULL
	uint32 FLAG_HAS_3_MOTV : 1;                     // = 0x40000000	    //Has three MOTV chunks, eg. for MOMT with shader 18.
	uint32 FLAG_UNK_0x80000000 : 1;                 // = 0x80000000
};

struct WMOGroupHeader
{
	uint32 groupName;
	uint32 descriptiveGroupName;
	WMOGroupFlags flags;
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

	uint32 unk0; // flags >= Mists
	uint32 unk1;

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

	uint8 materialId; // 0xff for collision

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

//---

class WMOGroup
{
public:
	WMOGroup(const WMO* _parentWMO, const uint32 m_GroupIndex, File& f);
	~WMOGroup();

	void Load();
	void initLighting();

	bool Render();
	bool drawDoodads(uint32 _doodadSet);
	bool drawLiquid();

	void setupFog();


public:
	const WMO* m_ParentWMO;
	const uint32 m_GroupIndex;
	string m_GroupName;
	WMOGroupHeader m_Header;

public:
	int fog;
	bool visible;
	bool m_EnableOutdoorLights;
	GLuint globalBuffer;
	BoundingBox m_Bounds;

public:
	//-- Triangles --//
	uint32 m_MaterialsInfoCount;
	WMOGroup_MaterialInfo* m_MaterialsInfo; // MOPY chunk

	uint16* m_Indices;                  // MOVI chunk

	uint32 m_VertexesCount;
	vec3* m_Vertexes;                   // MOVT chunk

	vec3* m_Normals;                    // MONR chunk
	vec2** m_TextureCoords;                  // MOTV chunk


	//-- Render bathes --//
	uint32 m_WMOBatchIndexesCount;
	WMOBatch* m_WMOBatchIndexes;      // MOBA chunk


	//-- Lights --//
	uint32 m_WMOLightsIndexesCount;
	uint16* m_WMOLightsIndexes;     // MOLR chunk


	//-- Doodads references --//
	uint32 m_DoodadsIndexesCount;
	uint16* m_DoodadsIndexes;       // MODR chunk


	//-- Collision --//
	// MOBN chunk
	// MOBR chunk

	//-- Vertex colors --//
	uint32** m_VertexColors;         // MOCV chunk

	//-- Liquid --//
	Liquid* m_Liquid;
};