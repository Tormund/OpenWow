#include "stdafx.h"

// General
#include "Engine.h"

// Additional
#include <ctime>
#include "GLFWBackend.h"
#include "DebugOutput_ConsoleWindows.h"

void shutdown(int _errCode)
{
	_Engine->Destroy();
}

bool Engine::Init(vector<string>& _argumentQueue)
{
	// Load static classes
	File::InitCriticalSect();
	Random::SetSeed(static_cast<unsigned long>(time(0)));

	// Add debug outputs
	Modules::log().AddDebugOutput(new DebugOutput_ConsoleWindows);
	//Modules::log().AddDebugOutput(new DebugOutput_Log);

	// Load graphics
	_GLFW->Init();

	Modules::log().Green("Engine[]: Loading.");

	// Arguments
	Modules::log().Print("Engine[]: Arguments count: [%d]", _argumentQueue.size());
	for (auto it = _argumentQueue.begin(); it != _argumentQueue.end(); ++it)
	{
		arguments.push_back(*it);
		Modules::log().Print("Engine[]: Argument: [%s]", (*it).c_str());
	}

	// Load modules
	_Render->Init();
	_TexturesMgr->Init();
	_FontsMgr->Init();
	_UIMgr->Init();

	// Add OpenGL console
	consoleOpenGL = new ConsoleOpenGL;
    Modules::log().AddDebugOutput(consoleOpenGL);

	// Add listener
	Modules::input().AddInputListener(consoleOpenGL);
	Modules::input().AddInputListener(_UIMgr);

	needExit = false;
	currentGameState = nullptr;

	framesCounter = 0;
	framesPerSecond = 0;
	framesTimer = 0;

	t = 0;

	return true;
}

void Engine::Destroy(uint32 _errorCode)
{
	Modules::log().Green("Engine[]: Destroy engine.");

	if (currentGameState != nullptr)
	{
		currentGameState->Destroy();
	}

	Modules::Destroy();

	exit(0);
}

bool Engine::SetGameState(GameState* _newGameState)
{
	Modules::log().Print("Engine[]: Setting new GameState.");

	if (_newGameState == nullptr)
	{
		Modules::log().Error("Engine[]: New GameState in null.");
		return false;
	}

	if (currentGameState != nullptr)
	{
		currentGameState->Destroy();
		delete currentGameState;
		Modules::log().Print("Engine[]: Current GameState destroyed.");
	}

	currentGameState = _newGameState;
	if (!currentGameState->IsInited())
	{
		Modules::log().Warn("Engine[]: New GameState in not inited. Initializating.");
		currentGameState->Init();
	}

	return true;
}

bool Engine::Tick()
{
	last_t = t;
	t = static_cast<uint32>(glfwGetTime() * 1000.0);
	uint32 dt = t - last_t;
	_time += dt;

	double dTime = static_cast<double>(_time) / 1000.0;
	double dDtTime = static_cast<double>(dt) / 1000.0;

	// Input
	if (currentGameState != nullptr)
	{
		currentGameState->InputPhase(dTime, dDtTime);
	}

	// Update
	if (currentGameState != nullptr)
	{
		currentGameState->UpdatePhase(dTime, dDtTime);
	}
	_UIMgr->Update();

	//

	_Render->r->beginRendering();

	// Render world
	_Render->Set3D();
	if (currentGameState != nullptr)
	{
		currentGameState->Render(dTime, dDtTime);
	}

	//

	// Render UI
	_Render->Set2D();
	if (currentGameState != nullptr)
	{
		currentGameState->RenderUI(dTime, dDtTime);
	}
	_UIMgr->RenderUI();
	consoleOpenGL->RenderUI();

	//

	// Swap buffers
	if (!_GLFW->SwapWindowBuffers())
	{
		if (!needExit)
		{
			Modules::log().Green("Engine[]: Need exit.");
			needExit = true;
			return false;
		}
	}

	// Caclulate FPS
	double currentTime = glfwGetTime();
	double delta = currentTime - framesTimer;
	framesCounter++;
	if (delta > 1.0)
	{
		framesPerSecond = static_cast<int>(static_cast<double>(framesCounter) / delta);
		framesTimer = currentTime;
		framesCounter = 0;

		_GLFW->SetWindowTitle("FPS: " + to_string(framesPerSecond));
	}

	return true;
}