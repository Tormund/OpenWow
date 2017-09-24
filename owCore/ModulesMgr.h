#pragma once

class Module;

class ModulesMgr
{
	CLASS_INSTANCE_API(ModulesMgr, );

public:
	 bool LoadModule(Module* _module, bool isSystemModule = false);
	 void DestroyModule(Module* _module);

	 void DestroyAllModules(bool _includeSystem = true);

	 bool IsModuleLoaded(Module* _module) const;

	template<class MODULE>
	inline MODULE* GetModule();

	inline size_t GetModulesCount() { return modules.size(); }

private:
	vector<Module*> modules;
};

#define _ModulesMgr ModulesMgr::instance()

#include "ModulesMgr.inl"
