#include "stdafx.h"

// Include
#include "GameState.h"

// General
#include "Engine.h"

// Additional
#include <ctime>
#include "ConsoleOpenGL.h"
#include "GameState_Empty.h"
#include "UIMgr.h"

void shutdown(int _errCode)
{
	_Engine->Destroy();
	Debug::Exit(_errCode);
}

bool Engine::Init(vector<string>& _argumentQueue)
{
	// Load static classes
	File::InitCriticalSect();
	Random::SetSeed(static_cast<unsigned long>(time(0)));


	// Add debug outputs
	Debug::AddDebugOutput(new DebugOutput_ConsoleWindows);
	//Debug::AddDebugOutput(new DebugOutput_Log);

	// Load graphics
	_ModulesMgr->LoadModule(_GLFW);

	Debug::Green("Engine[]: Loading.");

	// Arguments
	Debug::Print("Engine[]: Arguments count: [%d]", _argumentQueue.size());
	for (auto it = _argumentQueue.begin(); it != _argumentQueue.end(); ++it)
	{
		arguments.push_back(*it);
		Debug::Print("Engine[]: Argument: [%s]", (*it).c_str());
	}

	// Load modules
	_ModulesMgr->LoadModule(_Render);
	_ModulesMgr->LoadModule(_TexturesMgr);
	_ModulesMgr->LoadModule(_FontsMgr);
	_ModulesMgr->LoadModule(_Input);
	_ModulesMgr->LoadModule(_UIMgr);

	// Add OpenGL console
	//consoleOpenGL = new ConsoleOpenGL;
	//Debug::AddDebugOutput(consoleOpenGL);

	// Add listener
	//_Input->AddInputListener(consoleOpenGL);
	_Input->AddInputListener(_UIMgr);

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
	Debug::Green("Engine[]: Destroy engine.");

	if (currentGameState != nullptr)
	{
		currentGameState->Destroy();
	}

	_ModulesMgr->DestroyAllModules();

	Debug::Exit(_errorCode);
}

bool Engine::SetGameState(GameState* _newGameState)
{
	Debug::Print("Engine[]: Setting new GameState.");

	if (_newGameState == nullptr)
	{
		Debug::Error("Engine[]: New GameState in null.");
		return false;
	}

	if (currentGameState != nullptr)
	{
		currentGameState->Destroy();
		delete currentGameState;
		Debug::Print("Engine[]: Current GameState destroyed.");
	}

	currentGameState = _newGameState;
	if (!currentGameState->IsInited())
	{
		Debug::Warn("Engine[]: New GameState in not inited. Initializating.");
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
	//consoleOpenGL->RenderUI();

	//

	// Swap buffers
	if (!_GLFW->SwapWindowBuffers())
	{
		if (!needExit)
		{
			Debug::Green("Engine[]: Need exit.");
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