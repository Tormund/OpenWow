#pragma once

#include "Model_Instance.h"
#include "WowTypes.h"

class WMOGroup;
class WMOInstance;
class WMOManager;
class Liquid;

struct WMOFog;
struct WMOLight;
class WMOMaterial;

struct WMOHeader {
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

	//
	const size_t __size = 64;
};


/*struct WMOMaterial1 {
	int flags;
	int specular;
	int transparent; // Blending: 0 for opaque, 1 for transparent
	int nameStart; // Start position for the first texture filename in the MOTX data block	
	uint32_t col1; // color
	int d3; // flag
	int nameEnd; // Start position for the second texture filename in the MOTX data block
	uint32_t col2; // color
	int d4; // flag
	uint32_t col3;
	float f2;
	float diffColor[3];
	int texture1; // this is the first texture object. of course only in RAM. leave this alone. :D
	int texture2; // this is the second texture object.
	// read up to here -_-
	Texture* tex;
};*/

enum LightType {
	OMNI_LGT,
	SPOT_LGT,
	DIRECT_LGT,
	AMBIENT_LGT
};

struct WMOPV {
	vec3 a, b, c, d;
};

struct WMOPR {
	int16_t portal, group, dir, reserved;
};

struct WMODoodadSet {
	char name[0x14]; // Set name
	int32_t start; // index of first doodad instance in this set
	int32_t size; // number of doodad instances in this set
	int32_t unused; // always 0
};


class WMO : public RefItemNamed {
public:
	WMO(cstring name);
	~WMO() override;

	void draw(int doodadset, cvec3 ofs, const float rot);
	void drawPortals();
	void drawSkybox();

public:
	bool ok;

	vector<WMOGroup*> groups;
	vector<WMOMaterial*> mat;

	vector<string> models;
	vector<ModelInstance> modelis;

	vector<WMOLight> lights;
	vector<WMOPV> pvs;
	vector<WMOPR> prs;

	vector<WMOFog> fogs;

	vector<WMODoodadSet> doodadsets;

	WMOHeader header;

	Model * skybox;
};

/////////////////////////////////////

struct WMOPlacementInfo {
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
