#pragma once

// Additional
#include "EngineConfig.h"
#include "Input.h"

class Modules
{
public:
	static bool Init();
	static void Destroy();

	static EngineConfig&     config() { return *m_EngineConfig; }

private:
	static EngineConfig*     m_EngineConfig;
};
