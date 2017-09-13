#pragma once

#include "Model.h"

class ModelsManager : public Module, public RefManager1Dim<Model>
{
public:
	DEF_MODULE(ModelsManager, OW_MDX_DLL_API);

	Model* CreateAction(cstring name) override;
	bool DeleteAction(cstring name) override;

	void resetAnim();
	void updateEmitters(float dt);
};

#define _ModelsMgr ModelsManager::instance()