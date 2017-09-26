#pragma once

#include "WMO_Instance.h"

class Map_GlobalWMOs
{
public :
	Map_GlobalWMOs();
	~Map_GlobalWMOs();

	void SetGlobalWMOName(cstring _name);
	void SetGlobalWMOPlacementInfo(WMOPlacementInfo* _placement);

	void AddLowResWMOName(cstring _name);
	void AddLowResWMOPlacementInfo(WMOPlacementInfo* _placement);

	void InitGlobalsWMOs();


public:
	WMOInstance* GetGlobalWMOInstance() { return globalWMO; }
	bool IsGlobalWMOExists() { return globalWMOExists; }
	WMOPlacementInfo* GetGlobalWMOPlacementInfo() {	return globalWMOplacementInfo; }

private: // Global wmo
	bool globalWMOExists;
	string globalWMOName;
	WMOPlacementInfo* globalWMOplacementInfo;
	WMOInstance* globalWMO;

private: // Low-resolution WMOs
	uint32_t lowResolutionWMOsCount;
	vector<string> lowResolutionWMOsNames;
	vector<WMOPlacementInfo*> lowResolutionWMOsplacementInfo;
	vector<WMOInstance*> lowResolutionWMOs;
};