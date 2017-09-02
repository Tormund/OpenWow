#pragma once

class Module;

class ModulesMgr {
public:
	static OW_CORE_DLL_API ModulesMgr* instance() {
		static ModulesMgr instance;
		return &instance;
	}

	OW_CORE_DLL_API bool LoadModule(Module* _module, bool isSystemModule = false);
	OW_CORE_DLL_API void DestroyModule(Module* _module);

	OW_CORE_DLL_API void DestroyAllModules(bool _includeSystem = true);

	OW_CORE_DLL_API bool IsModuleLoaded(Module* _module) const;

	template<class MODULE>
	inline MODULE * GetModule();

	inline int GetModulesCount() { return modules.size(); }

private:
	vector<Module*> modules;
};

#define _ModulesMgr ModulesMgr::instance()

#include "ModulesMgr.inl"
