#pragma once

#include "MDX.h"

class ModelsManager : public Module, public RefManager1DimAssync<MDX>
{
public:
	DEF_MODULE(ModelsManager);

	// RefManager override
	MDX* CreateAction(cstring name) override;
	void LoadAction(string name, MDX* _model) override;
	bool DeleteAction(cstring name) override;

	void resetAnim();
	void updateEmitters(float dt);

	
};

#define _ModelsMgr ModelsManager::instance()