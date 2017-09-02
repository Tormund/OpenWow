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

	Debug::Info("WMOsManager: All WMOs destroyed.");
}

Model* ModelsManager::CreateAction(cstring name, uint32_t id)
{
	return new Model(name);
}

bool ModelsManager::DeleteAction(cstring name, uint32_t id)
{
	return true;
}

void ModelsManager::resetAnim()
{
	for (auto it = items.begin(); it != items.end(); ++it)
	{
		(it->second)->animcalc = false;
	}
}

void ModelsManager::updateEmitters(float dt)
{
	for (auto it = items.begin(); it != items.end(); ++it)
	{
		(it->second)->updateEmitters(dt);
	}
}