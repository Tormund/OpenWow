#pragma once

#include "Model.h"

class ModelsManager : public Module, public IntManager<Model>
{
public:
	DEF_MODULE_SIMPLE(ModelsManager);

	Model* CreateAction(cstring name, uint32_t id) override;
	bool DeleteAction(cstring name, uint32_t id) override;

	void resetAnim();
	void updateEmitters(float dt);
};

#define _ModelsMgr ModelsManager::instance()