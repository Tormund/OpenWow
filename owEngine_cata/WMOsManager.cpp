#include "stdafx.h"

// Includes

// General
#include "WMOsManager.h"

// Additional
#include "Wmo.h"

bool WMOsManager::Init()
{
	ADDCONSOLECOMMAND_CLASS("wmo_info", WMOsManager, PrintAllInfo);
	return true;
}

void WMOsManager::Destroy()
{
	DeleteAll();
	Debug::Info("WMOsManager: All WMOs destroyed.");
}

WMO* WMOsManager::CreateAction(cstring name)
{
	WMO* wmo = new WMO(name);

	if (!wmo->Init())
	{
		Debug::Error("WMOsManager[%s]: Failed init WMO.", name.c_str());
		delete wmo;
		return nullptr;
	}

	return wmo;
}

bool WMOsManager::DeleteAction(cstring name)
{
	return true;
}