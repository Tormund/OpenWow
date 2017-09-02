#include "stdafx.h"

// General
#include "Map_GlobalWMOs.h"

// Additional
#include "ModelsManager.h"
#include "WMOsManager.h"

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
}

void Map_GlobalWMOs::SetGlobalWMOName(cstring _name)
{
	globalWMOName = _name;

	//Debug::Info("World[%s]: WDT: Global WMO name [%s].", templateMap->Get_Directory_cstr(), globalWMOName.c_str());
}

void Map_GlobalWMOs::SetGlobalWMOPlacementInfo(WMOPlacementInfo * _placement)
{
	globalWMOExists = true;
	globalWMOplacementInfo = _placement;
}

void Map_GlobalWMOs::AddLowResWMOName(cstring _name)
{
	lowResolutionWMOsNames.push_back(_name);
	Debug::Info("Low-res name %s", _name.c_str());
}

void Map_GlobalWMOs::AddLowResWMOPlacementInfo(WMOPlacementInfo * _placement)
{
	lowResolutionWMOsplacementInfo.push_back(_placement);
}

void Map_GlobalWMOs::InitGlobalsWMOs()
{
	// Load global WMO
	//Debug::Info("World[%s]: Global WMO exists [%s].", templateMap->Get_Directory_cstr(), globalWMOExists ? "true" : "false");
	if (globalWMOExists)
	{
		WMO* wmo = _WMOsMgr->Add(globalWMOName);
		globalWMO = new WMOInstance(wmo, globalWMOplacementInfo);
	}

	// Load low-resolution WMOs
	//Debug::Info("World[%s]: Low WMOs count [%d].", templateMap->Get_Directory_cstr(), lowResolutionWMOsCount);
	for (size_t i = 0; i < lowResolutionWMOsCount; i++)
	{
		const string name = lowResolutionWMOsNames[lowResolutionWMOsplacementInfo[i]->nameIndex];

		WMO* wmo = _WMOsMgr->Add(name);
		WMOInstance* inst = new WMOInstance(wmo, lowResolutionWMOsplacementInfo[i]);
		lowResolutionWMOs.push_back(inst);
	}
}
