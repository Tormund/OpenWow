#include "stdafx.h"

// General
#include "Map_GlobalWMOs.h"

Map_GlobalWMOs::Map_GlobalWMOs()
{
	// Global WMO
	globalWMOExists = false;
	globalWMOName = "";
	globalWMOplacementInfo = nullptr;
	globalWMO = nullptr;

	// Low-resolution WMOs
	lowResolutionWMOsCount = 0;
}

Map_GlobalWMOs::~Map_GlobalWMOs()
{
	if (globalWMOExists)
	{
		_WMOsMgr->Delete(globalWMOName);
	}

	for (auto it = lowResolutionWMOsNames.begin(); it != lowResolutionWMOsNames.end(); ++it)
	{
		_WMOsMgr->Delete(*it);
	}
}

void Map_GlobalWMOs::SetGlobalWMOName(cstring _name)
{
	globalWMOName = _name;

	Debug::Green("Map_GlobalWMOs[]: Global WMO name [%s].", globalWMOName.c_str());
}

void Map_GlobalWMOs::SetGlobalWMOPlacementInfo(WMOPlacementInfo * _placement)
{
	globalWMOExists = true;
	globalWMOplacementInfo = _placement;
}

void Map_GlobalWMOs::AddLowResWMOName(cstring _name)
{
	lowResolutionWMOsNames.push_back(_name);
	Debug::Info("Map_GlobalWMOs: Low-res name [%s]", _name.c_str());
}

void Map_GlobalWMOs::AddLowResWMOPlacementInfo(WMOPlacementInfo * _placement)
{
	lowResolutionWMOsplacementInfo.push_back(_placement);
}

void Map_GlobalWMOs::InitGlobalsWMOs()
{
	// Load global WMO
	Debug::Info("Map_GlobalWMOs[]: Global WMO exists [%s].", globalWMOExists ? "true" : "false");
	if (globalWMOExists)
	{
		WMO* wmo = _WMOsMgr->Add(globalWMOName);
		globalWMO = new WMOInstance(wmo, globalWMOplacementInfo);
	}

	// Load low-resolution WMOs
	Debug::Info("Map_GlobalWMOs[]: Low WMOs count [%d].", lowResolutionWMOsCount);
	for (size_t i = 0; i < lowResolutionWMOsCount; i++)
	{
		const string name = lowResolutionWMOsNames[lowResolutionWMOsplacementInfo[i]->nameIndex];

		WMO* wmo = _WMOsMgr->Add(name);
		WMOInstance* inst = new WMOInstance(wmo, lowResolutionWMOsplacementInfo[i]);
		lowResolutionWMOs.push_back(inst);
	}
}
