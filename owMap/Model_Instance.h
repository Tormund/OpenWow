#pragma once

enum MDDFFlags
{
	mddf_biodome = 1,             // this sets internal flags to | 0x800 (WDOODADDEF.var0xC).
	mddf_shrubbery = 2,           // the actual meaning of these is unknown to me. maybe biodome is for really big M2s. 6.0.1.18179 seems not to check for this flag
	mddf_unk_4 = 0x4,             // Legion+ ᵘ
	mddf_unk_8 = 0x8,              // Legion+ ᵘ
	Flag_liquidKnown = 0x20,         // Legion+ ᵘ
	mddf_entry_is_filedata_id = 0x40, // Legion+ ᵘ mmidEntry is a file data id to directly load
	mddf_unk_100 = 0x100,         // Legion+ ᵘ
};


struct ModelPlacementInfo
{
	uint32 nameId;
	uint32 uniqueId;
	vec3 position;
	vec3 rotation;
	uint16 scale;
	uint16 flags;

	//
	static const uint32 __size = 36;
};

class ModelInstance
{
public:
	ModelInstance(File& f);
	~ModelInstance();

	void SetModel(MDX* m)
	{
		modelObject = m;
	}

	void draw();

public: 
	MDX* modelObject;
	ModelPlacementInfo* placementInfo;

	float sc;
};