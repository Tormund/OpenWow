#pragma once

struct WMOPlacementInfo
{
	uint32 nameIndex;
	uint32 uniqueId;
	vec3 position;
	vec3 rotation;
	CAaBox boundingBox;
	uint16 flags;
	uint16 doodadSetIndex;
	uint16 nameSet;
	uint16 unk;

	// Size in bytes
	static const uint32 __size = 64;
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

	static std::set<uint32> alreadyDraw;
};