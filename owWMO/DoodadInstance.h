#pragma once

class MDX;

struct DoodadPlacementInfo
{
	struct
	{
		/*000h*/  uint32_t nameIndex : 24;          // reference offset into MODN
		/*003h*/  uint32_t flag_AcceptProjTex : 1;
		/*003h*/  uint32_t flag_0x2 : 1;            // MapStaticEntity::field_34 |= 1 (if set, MapStaticEntity::AdjustLighting is _not_ called)
		/*003h*/  uint32_t flag_0x4 : 1;
		/*003h*/  uint32_t flag_0x8 : 1;
		/*003h*/  uint32_t : 4;                     // unused as of 7.0.1.20994
	} flags;

	/*004h*/  C3Vector position;               // (X,Z,-Y)
	/*010h*/  C4ImQuaternion orientation;        // (X, Y, Z, W)
	/*020h*/  float scale;                      // scale factor
	/*024h*/  CImVector color;                 // (B,G,R,A) diffuse lighting color, used in place of global diffuse from DBCs

	//
	static const uint32_t __size = 40;
};

class DoodadInstance
{
public:
	DoodadInstance(File& f);
	~DoodadInstance();

	void SetModel(MDX* m)
	{
		modelObject = m;
	}

	void Draw(cvec3 ofs, float roll);

public:
	MDX* modelObject;
	DoodadPlacementInfo* placementInfo;

	int light;
	vec3 ldir;
	vec4 lcol;
};