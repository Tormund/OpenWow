#pragma once

class Module;

class ModulesMgr
{
	CLASS_INSTANCE_API(ModulesMgr, OW_CORE_DLL_API);

public:
	OW_CORE_DLL_API bool LoadModule(Module* _module, bool isSystemModule = false);
	OW_CORE_DLL_API void DestroyModule(Module* _module);

	OW_CORE_DLL_API void DestroyAllModules(bool _includeSystem = true);

	OW_CORE_DLL_API bool IsModuleLoaded(Module* _module) const;

	template<class MODULE>
	inline MODULE* GetModule();

	inline size_t GetModulesCount() { return modules.size(); }

private:
	vector<Module*> modules;
};

#define _ModulesMgr ModulesMgr::instance()

#include "ModulesMgr.inl"
