#pragma once

#include "Wmo.h"

class WMOsManager : public Module, public RefManager1DimAssync<WMO>
{
public:
	DEF_MODULE(WMOsManager);

	WMO* CreateAction(cstring name) override;
	void LoadAction(string name, WMO* _wmo) override;
	bool DeleteAction(cstring name) override;
};

#define _WMOsMgr WMOsManager::instance()