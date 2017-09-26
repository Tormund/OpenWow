#pragma once

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
	uint16_t unk;

	// Size in bytes
	static const uint32_t __size = 64;
};

class WMOInstance
{
public:
	WMOInstance(WMO* _wmoObject, File& f);
	WMOInstance(WMO* _wmoObject, WMOPlacementInfo* _placementInfo);
	~WMOInstance();

	void SetWMO(WMO* m)
	{
		wmoObject = m;
	}

	void draw();

	static void reset();

	inline WMO* GetWMO() { return wmoObject; }

private:
	WMO* wmoObject;
	WMOPlacementInfo* placementInfo;

	static std::set<uint32_t> alreadyDraw;
};