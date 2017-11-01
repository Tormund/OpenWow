#pragma once

#include "GameState.h"
#include "OpenGLAdapter.h"

class Engine
{
	CLASS_INSTANCE(Engine);

public:
	bool PreInit(int argumentCount, char* arguments[]);
	bool Init();
	void Destroy(uint32 _errorCode = 0);

	bool SetAdapter(OpenGLAdapter* _openGLAdapter);
	OpenGLAdapter* GetAdapter() { return m_OpenGLAdapter; }

	bool SetGameState(GameState* _newGameState);
	GameState* GetGameState() const { return currentGameState; }

	bool Tick();

	string GetArgument(uint8 index) 
	{ 
		if (m_Arguments.size() > (index + 1))
		{
			return m_Arguments[index + 1];
		}
		else
		{
			return "";
		}
	}
	

	uint32 GetFPS() const { return framesPerSecond; }

	

private:
	vector<string> m_Arguments;
	bool needExit;
	GameState* currentGameState;

	uint32 framesCounter, framesPerSecond;
	double framesTimer;

	uint32 t;
	uint32 last_t;
	uint32 _time;

	OpenGLAdapter* m_OpenGLAdapter;
	//ConsoleOpenGL* consoleOpenGL;
};

#define _Engine Engine::instance()
