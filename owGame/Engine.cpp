#include "stdafx.h"

// General
#include "Engine.h"

// Additional
#include <ctime>
#include "GUI\UIMgr.h"

void shutdown(int _errCode)
{
	_Engine->Destroy();
}

bool Engine::PreInit(int argumentCount, char* arguments[])
{
	// Arguments
	for (int i = 0; i < argumentCount; i++)
	{
		m_Arguments.push_back(arguments[i]);
	}

	// System settings
	File::InitCriticalSect();
	Random::SetSeed(static_cast<unsigned long>(time(0)));

	// Modules manager
	Modules::Init();

	// Files
	MPQArchiveStorage::InitCommonArchives();

	return true;
}

bool Engine::Init()
{
	Modules::log().Green("Engine[]: Loading.");

	// Init adapter
	assert1(m_OpenGLAdapter);
	assert1(m_OpenGLAdapter->Init());

	// Load modules
	assert1(_Render->Init());
	assert1(_TexturesMgr->Init());
	assert1(_FontsMgr->Init());
	assert1(_UIMgr->Init());

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

	MPQArchiveStorage::ClearArchives();
	Modules::Destroy();

	exit(0);
}

bool Engine::SetAdapter(OpenGLAdapter * _openGLAdapter)
{
	assert1(_openGLAdapter != nullptr);
	m_OpenGLAdapter = _openGLAdapter;
	return true;
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
	t = static_cast<uint32>(m_OpenGLAdapter->GetTime() * 1000.0);
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
	_Render->r->clear();

	// Render world
	_Render->Set3D();
	if (currentGameState != nullptr)
	{
		currentGameState->Render(dTime, dDtTime);
	}

	//

	// Render UI
	_Render->Set2D();
	RenderableUIObjectCollection::RenderUI();

	//

	// Swap buffers
	if (!m_OpenGLAdapter->SwapWindowBuffers())
	{
		if (!needExit)
		{
			Modules::log().Green("Engine[]: Need exit.");
			needExit = true;
			return false;
		}
	}

	// Caclulate FPS
	double currentTime = m_OpenGLAdapter->GetTime();
	double delta = currentTime - framesTimer;
	framesCounter++;
	if (delta > 1.0)
	{
		framesPerSecond = static_cast<int>(static_cast<double>(framesCounter) / delta);
		framesTimer = currentTime;
		framesCounter = 0;

		m_OpenGLAdapter->SetWindowTitle("FPS: " + to_string(framesPerSecond));
	}

	return true;
}