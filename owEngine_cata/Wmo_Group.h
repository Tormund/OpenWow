#pragma once

class WMO;
class Liquid;

class WMOGroup
{
	struct WMOGroupInfo
	{
		enum WMOGroupInfoFlags
		{
			FLAG_HAS_COLLISION = 0x1,					//Has MOBN and MOBR chunk.
			FLAG_HAS_LIGHTMAP = 0x2,					//Has light map MOLM, MOLD
			FLAG_HAS_VERTEX_COLORS = 0x4, 				//Has vertex colors(MOCV chunk).
			FLAG_IS_OUTDOOR = 0x8, 						//SMOGroup::EXTERIOR -- Outdoor
			//0x10
			//0x20
			FLAG_DO_NOT_USE_LIGHTING_DIFFUSE = 0x40,    //Do not use local diffuse lightning .Applicable for both doodads from this wmo group(color from MODD) and water(CWorldView::GatherMapObjDefGroupLiquids).
			FLAG_IS_UNREACHABLE = 0x80, 				//SMOGroup::UNREACHABLE
			//0x100
			FLAG_HAS_LIGHTS = 0x200, 					//Has lights (MOLR chunk)
			//0x400										//Cataclysm: Has MPBV, MPBP, MPBI, MPBG chunks, neither 3.3.5a nor Cataclysm alpha actually use them though, but just skips them.Legion + (? ) : SMOGroup::LOD : Also load for LoD != 0 (_lod* groups)
			FLAG_HAS_DOODADS = 0x800, 					//Has doodads(MODR chunk)
			FLAG_HAS_WATER = 0x1000,					//SMOGroup::LIQUIDSURFACE -- Has water(MLIQ chunk)
			FLAG_IS_INDOOR = 0x2000,					//SMOGroup::INTERIOR -- Indoor
			//0x4000
			//0x8000
			FLAG_ALWAYS_DRAW = 0x10000,					//SMOGroup::ALWAYSDRAW -- clear 0x8 after CMapObjGroup::Create() in MOGP and MOGI
			//0x20000									//Has MORI and MORB chunks.
			FLAG_HAS_SKYBOX = 0x40000,					//Show skybox -- automatically unset if MOSB not present.
			//0x80000									//is_not_water_but_ocean, LiquidType related, see below in the MLIQ chunk.
			//0x100000
			//0x200000									//IsMountAllowed
			//0x400000
			//0x800000
			FLAG_HAS_2_VERTEX_COLORS_BUFFERS = 0x1000000, //SMOGroup::CVERTS2: Has two MOCV chunks : Just add two or don't set 0x4 to only use cverts2.
			FLAG_HAS_2_TEXTURES_COORDS = 0x2000000,		//SMOGroup::TVERTS2 : Has two MOTV chunks : Just add two.
			//0x4000000									//SMOGroup::ANTIPORTAL : Just call CMapObjGroup::CreateOccluders() independent of groupname being "antiportal".requires intBatchCount == 0, extBatchCount == 0, UNREACHABLE.
			//0x8000000									//unk.requires intBatchCount == 0, extBatchCount == 0, UNREACHABLE.
			//0x10000000
			//0x20000000
			FLAG_HAS_3_TEXTURES_COORDS = 0x40000000,	//SMOGroup::TVERTS3 : Has three MOTV chunks, eg. for MOMT with shader 18.
			//0x80000000
		};

		uint32_t flags;      //  see information in in MOGP, they are equivalent
		CAaBox bounding_box;
		int32_t nameoffset;  // name in MOGN chunk (-1 for no name)

		//
		static const size_t __size = 32;
	};

	WMO* wmo;

	uint32_t nTriangles, nVertices;
	vec3 center;
	float rad;
	int num;
	int fog;
	uint32_t nDoodads;
	uint32_t nBatches;
	short *ddr;
	Liquid *lq;
	vector< pair<GLuint, int> > lists;

public:
	vec3 b1, b2;
	vec3 vmin, vmax;
	bool indoor, hascv;
	bool visible;

	bool outdoorLights;
	string name;


public:
	WMOGroup();
	~WMOGroup();

	void init(WMO* wmo, File& f, int num, char* names);
	void initDisplayList();
	void initLighting(int nLR, short *useLights);

	void draw(cvec3 ofs, const float rot);
	void drawDoodads(int doodadset, cvec3 ofs, const float rot);
	void drawLiquid();

	void setupFog();
};