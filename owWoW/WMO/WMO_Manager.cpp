#include "../stdafx.h"

// Includes

// General
#include "WMO_Manager.h"

// Additional
#include "Wmo.h"

bool WMOsManager::Init()
{
	ADDCONSOLECOMMAND_CLASS("wmo_info", WMOsManager, PrintAllInfo);

	RefManager1DimAssync::Init();

	return true;
}

void WMOsManager::Destroy()
{
	RefManager1DimAssync::Destroy();

	DeleteAll();

	Log::Info("WMOsManager[]: All WMOs destroyed.");
}

WMO* WMOsManager::CreateAction(cstring name)
{
	WMO* _wmo = new WMO(name);
	return _wmo;
}

void WMOsManager::LoadAction(string name, WMO* _wmo)
{
	//wglMakeCurrent(_Render->dc, _Render->glrc4);

	if (!_wmo->Load())
	{
		Log::Error("WMOsManager[%s]: Failed init WMO.", name.c_str());
	}
}

bool WMOsManager::DeleteAction(cstring name)
{
	return true;
}