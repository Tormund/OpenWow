#pragma once

class WMOInstance {
public:
	WMOInstance(WMO* _wmoObject, WMOPlacementInfo* _placementInfo);
	~WMOInstance();

	void draw();

	static void reset();

private:
	WMO* wmoObject;
	WMOPlacementInfo* placementInfo;

	static std::set<uint32_t> alreadyDraw;
};