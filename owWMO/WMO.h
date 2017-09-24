#pragma once

#include "DoodadInstance.h"

class WMOGroup;
class WMOInstance;
class WMOManager;
class Liquid;

class WMOFog;
class WMOLight;
class WMOMaterial;

struct WMOHeaderDef
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

	struct
	{
		uint16_t FLAG_attenuate_vertices_based_on_distance_to_portal : 1;
		uint16_t FLAG_skip_base_color : 1;                       // do not add base (ambient) color (of MOHD) to MOCVs. apparently does more, e.g. required for multiple MOCVs
		uint16_t FLAG_use_liquid_type_dbc_id : 1;                // use real liquid type ID from DBCs instead of local one. See MLIQ for further reference.
		uint16_t FLAG_lighten_interiors : 1;                     // makes iterior groups much brighter, effects MOCV rendering. Used e.g.in Stormwind for having shiny bright interiors,
		uint16_t FLAG_Lod : 1;                                   // ≥ Legion (20740)
		uint16_t : 11;                                           // unused as of Legion (20994)
	} flags;

	uint16_t numLod;                                             // ≥ Legion (21108) includes base lod (→ numLod = 3 means '.wmo', 'lod0.wmo' and 'lod1.wmo')

	//
	static const size_t __size = 64;
};

struct WMO_PortalVertices
{
	vec3 a, b, c, d;
};

struct WMO_PortalInformation
{
	uint16_t startVertex;
	uint16_t count;
	C4Plane plane;

	//
	const static size_t __size = 20;
};

struct WMO_PortalReferences
{
	uint16_t portalIndex;  // into MOPR
	uint16_t groupIndex;   // the other one
	int16_t side;          // positive or negative.
	uint16_t filler;

	//
	const static size_t __size = 8;
};

struct WMO_DoodadSet
{
	char name[20];  // Set name
	uint32_t start; // index of first doodad instance in this set
	uint32_t size;  // number of doodad instances in this set
	char pad[4];    // always 0

	inline bool InSet(uint32_t _index)
	{
		return ((_index >= start) && (_index < (start + size)));
	}

	//
	const static size_t __size = 32;
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
	WMOHeaderDef header;                                   // MOHD chunk

	//-- Materials --//
	char* texbuf;                                          // MOTX chunk
	vector<WMOMaterial*> mat;                              // MOMT chunk


	//-- Groups --//
	char* groupnames;									   // MOGN chunk
	vector<WMOGroup*> groups;                              // MOGI chunk


	//-- Skybox --//
	char* skybox_filename;                                 // MOSB chunk
	Model* skybox;


	//-- Portals --//
	vector<WMO_PortalVertices> m_PortalVertices;           // MOPV chunk
	vector<WMO_PortalInformation> m_PortalInformation;     // MOPT chunk
	vector<WMO_PortalReferences> m_PortalReferences;       // MOPR chunk
	// MOVV chunk
	// MOVB chunk


	// -- Lights --//
	vector<WMOLight> lights;                               // MOLT chunk


	//-- Doodads --//
#ifdef MDX_INCL
	vector<WMO_DoodadSet> doodadsets;                      // MODS chunk
	char* m_MDXFilenames;                                  // MODN chunk
	vector<string> m_MDXNames;                             
	vector<DoodadInstance> m_MDXInstances;                 // MODD chunk
#endif


	//-- Fog --//
	vector<WMOFog> fogs;                                   // MFOG chunk


	//-- Volumes plane --//

														   // MCVP chunk (optional)	
};
