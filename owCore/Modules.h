#pragma once

// Additional
#include "EngineConfig.h"
#include "EngineLog.h"
class StatManager;
class SceneManager;
class ResourceManager;
class Renderer;
#include "Input.h"

class Modules
{
public:
	static bool Init();
	static void Destroy();

	static void SetError(const char* errorStr1 = nullptr, const char* errorStr2 = nullptr);
	static bool GetError();

	static EngineConfig&     config() { return *m_EngineConfig; }
	static EngineLog&        log() { return *m_EngineLog; }
	static StatManager&      stats() { return *m_StatManager; }
	static SceneManager&     sceneMan() { return *m_SceneManager; }
	static ResourceManager&  resMan() { return *m_ResourceManager; }
	static Renderer&         renderer() { return *m_Renderer; }
	static Input&            input() { return *m_Input; }

private:
	static bool              m_ErrorFlag;

	static EngineConfig*     m_EngineConfig;
	static EngineLog*        m_EngineLog;
	static StatManager*      m_StatManager;
	static SceneManager*     m_SceneManager;
	static ResourceManager*  m_ResourceManager;
	static Renderer*         m_Renderer;

	static Input*            m_Input;
};
