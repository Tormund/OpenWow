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

#include "../RenderableItem.h"
#include "../SceneNode.h"
#include "../WMO/WMO.h"

class WMOInstance : protected SceneNode, protected RenderableItem
{
public:
	WMOInstance(WMO* _wmoObject, File& f);
	WMOInstance(WMO* _wmoObject, WMOPlacementInfo* _placementInfo);
	~WMOInstance();

	void SetWMO(WMO* _wmo) { wmoObject = _wmo; }
	WMO* GetWMO() { return wmoObject; }

	void Render();

private:
	void CalculateMatrix();

private:
	WMO* wmoObject;
	WMOPlacementInfo* placementInfo;

	static std::set<uint32> alreadyDraw;


public:	// Static
	static void reset();
};