#include "stdafx.h"

// General
#include "Modules.h"

// Static
bool             Modules::m_ErrorFlag = false;
EngineConfig*    Modules::m_EngineConfig = nullptr;
EngineLog*       Modules::m_EngineLog = nullptr;
StatManager*     Modules::m_StatManager = nullptr;
SceneManager*    Modules::m_SceneManager = nullptr;
ResourceManager* Modules::m_ResourceManager = nullptr;
Renderer*        Modules::m_Renderer = nullptr;

Input*           Modules::m_Input = nullptr;

bool Modules::Init()
{
	if (m_EngineLog == nullptr)
	{
		m_EngineLog = new EngineLog();
	}

	if (m_EngineConfig == nullptr)
	{
		m_EngineConfig = new EngineConfig();
	}

	/*if (m_SceneManager == nullptr)
	{
		m_SceneManager = new SceneManager();
	}

	if (m_ResourceManager == nullptr)
	{
		m_ResourceManager = new ResourceManager();
	}

	if (m_Renderer == nullptr)
	{
		m_Renderer = new Renderer();
	}

	if (m_StatManager == nullptr)
	{
		m_StatManager = new StatManager();
	}*/

	if (m_Input == nullptr)
	{
		m_Input = new Input();
	}

	return true;
}

void Modules::Destroy()
{

}

void Modules::SetError(const char *errorStr1, const char *errorStr2)
{
	static string msg;
	msg.resize(0);

	if (errorStr1)
	{
		msg.append(errorStr1);
	}

	if (errorStr2)
	{
		msg.append(errorStr2);
	}

	m_ErrorFlag = true;
	m_EngineLog->Error(msg.c_str());
}

bool Modules::GetError()
{
	if (m_ErrorFlag)
	{
		m_ErrorFlag = false;
		return true;
	}
	else
	{
		return false;
	}
}