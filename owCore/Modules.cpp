#include "stdafx.h"

// General
#include "Modules.h"

// Additional
#include "Log.h"
#include "Input.h"

// Static
EngineConfig*    Modules::m_EngineConfig = nullptr;

bool Modules::Init()
{
    assert1(Log::Init());

	if (m_EngineConfig == nullptr)
	{
		m_EngineConfig = new EngineConfig();
	}

	return true;
}

void Modules::Destroy()
{

}