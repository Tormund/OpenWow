#include "stdafx.h"

// General
#include "ModelsManager.h"

// Additional
#include "Model.h"

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

	Debug::Info("MDXsManager: All MDXs destroyed.");
}

//

Model* ModelsManager::CreateAction(cstring name)
{
	return new Model(name);
}

void ModelsManager::LoadAction(string name, Model* _model)
{
	wglMakeCurrent(_Render->dc, _Render->glrc3);

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