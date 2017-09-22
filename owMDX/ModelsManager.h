#pragma once

#include "Model.h"

class ModelsManager : public Module, public RefManager1DimAssync<Model>
{
public:
	DEF_MODULE(ModelsManager);

	Model* CreateAction(cstring name) override;
	void LoadAction(string name, Model* _model) override;
	bool DeleteAction(cstring name) override;

	void resetAnim();
	void updateEmitters(float dt);

	
};

#define _ModelsMgr ModelsManager::instance()