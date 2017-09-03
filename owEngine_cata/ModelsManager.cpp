#include "stdafx.h"

// General
#include "ModelsManager.h"

// Additional
#include "Model.h"

bool ModelsManager::Init()
{
	ADDCONSOLECOMMAND_CLASS("models_info", ModelsManager, PrintAllInfo);

	return true;
}

void ModelsManager::Destroy()
{
	DeleteAll();

	Debug::Info("MDXsManager: All MDXs destroyed.");
}

Model* ModelsManager::CreateAction(cstring name)
{
	return new Model(name);
}

bool ModelsManager::DeleteAction(cstring name)
{
	return true;
}

void ModelsManager::resetAnim()
{
	for (auto it = objects.begin(); it != objects.end(); ++it)
	{
		(it->second)->animcalc = false;
	}
}

void ModelsManager::updateEmitters(float dt)
{
	for (auto it = objects.begin(); it != objects.end(); ++it)
	{
		(it->second)->updateEmitters(dt);
	}
}