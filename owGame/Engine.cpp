#include "stdafx.h"

// Include
#include "GameState.h"

// General
#include "Engine.h"

// Additional
#include <ctime>
#include "ConsoleOpenGL.h"
#include "GameState_Empty.h"

void shutdown(int _errCode)
{
	_Engine->Destroy();
	Debug::Exit(_errCode);
}

bool Engine::Init(vector<string>& _argumentQueue)
{
	File::InitCriticalSect();

	// Add debug outputs
	Debug::AddDebugOutput(new DebugOutput_ConsoleWindows);
	Debug::AddDebugOutput(new DebugOutput_Log);

	// Load graphics
	_ModulesMgr->LoadModule(_GLFW, true);

	Debug::Green("Engine[]: Loading.");

	// Arguments
	Debug::Print("Engine[]: Arguments count: [%d]", _argumentQueue.size());
	for (auto it = _argumentQueue.begin(); it != _argumentQueue.end(); ++it)
	{
		arguments.push_back(*it);
		Debug::Print("Engine[]: Argument: [%s]", (*it).c_str());
	}

	// Load static classes
	Debug::Init();
	Random::SetSeed(static_cast<unsigned long>(time(0)));

	// Load modules
	_ModulesMgr->LoadModule(_Render, true);
	//_ModulesMgr->LoadModule(_FileSystem, true);
	_ModulesMgr->LoadModule(_TexturesMgr, true);
	_ModulesMgr->LoadModule(_FontsMgr, true);
	_ModulesMgr->LoadModule(_Input, true);
	_ModulesMgr->LoadModule(_UIMgr, true);

	// Add OpenGL console
	consoleOpenGL = new ConsoleOpenGL;
	Debug::AddDebugOutput(consoleOpenGL);

	// Add listener
	_Input->AddInputListener(consoleOpenGL);
	_Input->AddInputListener(_UIMgr);

	needExit = false;
	currentGameState = nullptr;

	framesCounter = 0;
	framesPerSecond = 0;
	framesTimer = 0;

	t = 0;

	return true;
}

void Engine::Destroy(uint32_t _errorCode)
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
	if (currentGameState == nullptr)
	{
		Debug::Warn("Egnine[]: Current game state is null. Set empty GameState.");
		SetGameState(new GameState_Empty);
	}

	last_t = t;
	t = GetTicks();
	uint32_t dt = t - last_t;
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
	consoleOpenGL->RenderUI();

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
	double currentTime = _GLFW->GetTimeSeconds();
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

//---------------------------------------------------------

unsigned long long Engine::GetTicks() const
{
	return static_cast<unsigned long long>(_GLFW->GetTimeSeconds() * 1000.0);
}