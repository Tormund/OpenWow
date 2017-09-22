#pragma once

#include "Model_Instance.h"

class WMOGroup;
class WMOInstance;
class WMOManager;
class Liquid;

struct WMOFog;
struct WMOLight;
class WMOMaterial;

struct WMOHeader
{
	uint32_t nTextures;
	uint32_t nGroups;
	uint32_t nPortals;
	uint32_t nLights;
	uint32_t nDoodadNames;
	uint32_t nDoodadDefs;
	uint32_t nDoodadSets;
	CArgb ambColor;				// Color settings for base (ambient) color. See the flag at /*03Ch*/.   
	uint32_t wmoID;				// &WMOAreaTableRec::m_WMOID
	CAaBox bounding_box;

	uint16_t flag_attenuate_vertices_based_on_distance_to_portal : 1;
	uint16_t flag_skip_base_color : 1;                       // do not add base (ambient) color (of MOHD) to MOCVs. apparently does more, e.g. required for multiple MOCVs
	uint16_t flag_use_liquid_type_dbc_id : 1;                // use real liquid type ID from DBCs instead of local one. See MLIQ for further reference.
	uint16_t flag_lighten_interiors : 1;                     // makes iterior groups much brighter, effects MOCV rendering. Used e.g.in Stormwind for having shiny bright interiors,
	uint16_t Flag_Lod : 1;                                   // ≥ Legion (20740)
	uint16_t : 11;                                           // unused as of Legion (20994)
	
	uint16_t numLod;                                         // ≥ Legion (21108) includes base lod (→ numLod = 3 means '.wmo', 'lod0.wmo' and 'lod1.wmo')

	//

	static const size_t __size = 64;
};


enum LightType
{
	OMNI_LGT,
	SPOT_LGT,
	DIRECT_LGT,
	AMBIENT_LGT
};

struct WMOPV
{
	vec3 a, b, c, d;
};

struct WMOPR
{
	int16_t portal, group, dir, reserved;
};

struct WMODoodadSet
{
	char name[0x14]; // Set name
	int32_t start; // index of first doodad instance in this set
	int32_t size; // number of doodad instances in this set
	int32_t unused; // always 0
};


class WMO : public RefItemNamed
{
public:
	WMO(cstring _fileName);
	~WMO();

	bool Init();

	void draw(int doodadset, cvec3 ofs, const float roll);
	void drawPortals();
	void drawSkybox();

public:
	vector<WMOGroup*> groups;
	vector<WMOMaterial*> mat;

#ifdef MDX_INCL
	vector<string> m_MDXNames;
	vector<ModelInstance> m_MDXInstances;
#endif // MDX_INCL

	vector<WMOLight> lights;

	vector<WMOPV> pvs;
	vector<WMOPR> prs;

	vector<WMOFog> fogs;

	vector<WMODoodadSet> doodadsets;

	WMOHeader header;

	Model* skybox;
};

/////////////////////////////////////

struct WMOPlacementInfo
{
	uint32_t nameIndex;
	uint32_t uniqueId;
	vec3 position;
	vec3 rotation;
	CAaBox boundingBox;
	uint16_t flags;
	uint16_t doodadSetIndex;
	uint16_t nameSet;
	uint16_t padding;

	// Size in bytes
	static const size_t __size = 64;
};
