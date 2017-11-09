#include "stdafx.h"

// General
#include "Engine.h"

// Additional
#include <ctime>
#include "GameStateManager.h"

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
	Log::Green("Engine[]: Loading.");

	// Init adapter
	assert1(m_OpenGLAdapter);
	assert1(m_OpenGLAdapter->Init());

	// Load modules
	assert1(_Render->Init());
	assert1(_TexturesMgr->Init());
	assert1(_FontsMgr->Init());

	needExit = false;

	framesCounter = 0;
	framesPerSecond = 0;
	framesTimer = 0;

	t = 0;

	return true;
}

void Engine::Destroy(uint32 _errorCode)
{
	Log::Green("Engine[]: Destroy engine.");

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

bool Engine::Tick()
{
	last_t = t;
	t = static_cast<uint32>(m_OpenGLAdapter->GetTime() * 1000.0);
	uint32 dt = t - last_t;
	_time += dt;

	double dTime = static_cast<double>(_time) / 1000.0;
	double dDtTime = static_cast<double>(dt) / 1000.0;

    //------------------------------------------------
	//-- Update
    //------------------------------------------------
    UpdatableObjectCollection::Update(dTime, dDtTime);

	//

	_Render->r->beginRendering();
	_Render->r->clear();

    //------------------------------------------------
    //-- Render3D
    //------------------------------------------------
	_Render->Set3D();
	if (GameStateManager::GetGameState() != nullptr)
	{
        GameStateManager::GetGameState()->Render(dTime, dDtTime);
	}

    //------------------------------------------------
    //-- RenderUI
    //------------------------------------------------
	_Render->Set2D();
	RenderableUIObjectCollection::RenderUI();

	//

	// Swap buffers
	if (!m_OpenGLAdapter->SwapWindowBuffers())
	{
		if (!needExit)
		{
			Log::Green("Engine[]: Need exit.");
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