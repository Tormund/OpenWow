#include "stdafx.h"

// General
#include "ModulesMgr.h"

bool ModulesMgr::LoadModule(Module* _module)
{
	assert1(_module != nullptr);

	if (!_module->Init())
	{
		Debug::Error("Module [%s] failed init.", _module->GetName().c_str());
		return false;
	}

	_module->SetInited();

	modules.push_back(_module);

	Debug::Print("Module [%s] inited.", _module->GetName().c_str());

	return true;
}

void ModulesMgr::DestroyModule(Module* _module)
{
	assert1(_module != nullptr);

	Debug::Print("Module [%s] destroying...", _module->GetName().c_str());
	_module->Destroy();
	Debug::Print("Module [%s] destroyed.", _module->GetName().c_str());

	modules.erase(std::remove(modules.begin(), modules.end(), _module), modules.end());
}

void ModulesMgr::DestroyAllModules()
{
	while (!modules.empty())
	{
		auto _module = modules.back();
		DestroyModule(_module);
	}
}

bool ModulesMgr::IsModuleLoaded(Module* _module) const
{
	return find(modules.begin(), modules.end(), _module) != modules.end();
}
