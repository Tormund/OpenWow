#pragma once

#include "Wmo.h"

class WMOsManager : public Module, public RefManager1Dim<WMO> {
public:
	DEF_MODULE_SIMPLE(WMOsManager);

	WMO* CreateAction(cstring name) override;
	bool DeleteAction(cstring name) override;
};

#define _WMOsMgr WMOsManager::instance()