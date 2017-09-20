#include "stdafx.h"

// General
#include "ModulesMgr.h"

bool ModulesMgr::LoadModule(Module* _module, bool isSystemModule)
{
	assert1(_module != nullptr);

	Debug::Green("Module [%s] initializating...", _module->GetName().c_str());

	if (!_module->Init())
	{
		Debug::Error("Module [%s] failed init.", _module->GetName().c_str());
		return false;
	}

	_module->SetInited();

	if (isSystemModule)
	{
		_module->SetSystemModule();
	}

	modules.push_back(_module);

	Debug::Green("Module [%s] inited.", _module->GetName().c_str());
	return true;
}

void ModulesMgr::DestroyModule(Module* _module)
{
	assert1(_module != nullptr);

	Debug::Green("Module [%s] destroying...", _module->GetName().c_str());
	_module->Destroy();
	Debug::Green("Module [%s] destroyed.", _module->GetName().c_str());

	modules.erase(std::remove(modules.begin(), modules.end(), _module), modules.end());
}

void ModulesMgr::DestroyAllModules(bool _includeSystem)
{
	while (!modules.empty())
	{
		auto _module = modules.back();

		if (!_includeSystem && _module->IsSystemModule())
		{
			continue;
		}

		DestroyModule(_module);
	}
}

bool ModulesMgr::IsModuleLoaded(Module* _module) const
{
	return find(modules.begin(), modules.end(), _module) != modules.end();
}
