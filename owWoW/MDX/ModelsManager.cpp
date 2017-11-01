#include "../stdafx.h"

// General
#include "ModelsManager.h"

// Additional
#include "MDX.h"

bool ModelsManager::Init()
{
	ADDCONSOLECOMMAND_CLASS("models_info", ModelsManager, PrintAllInfo);

	RefManager1DimAssync::Init();

	return true;
}

void ModelsManager::Destroy()
{
	RefManager1DimAssync::Destroy();

	DeleteAll();

	Modules::log().Info("MDXsManager[]: All MDXs destroyed.");
}

//

MDX* ModelsManager::CreateAction(cstring name)
{
	MDX* model = new MDX(name);
	return model;
}

void ModelsManager::LoadAction(string name, MDX* _model)
{
	//wglMakeCurrent(_Render->dc, _Render->glrc3);

	_model->Init();
}

bool ModelsManager::DeleteAction(cstring name)
{
	return true;
}

void ModelsManager::resetAnim()
{
	for (auto it = objects.begin(); it != objects.end(); ++it)
	{
		if (it->second != nullptr) // FIXME: Incorrect names in objects
		{
			(it->second)->animcalc = false;
		}
	}
}

void ModelsManager::updateEmitters(float dt)
{
	for (auto it = objects.begin(); it != objects.end(); ++it)
	{
		if (it->second != nullptr)  // FIXME: Incorrect names in objects
		{
			(it->second)->updateEmitters(dt);
		}
	}
}