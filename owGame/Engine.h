#pragma once

class ConsoleOpenGL;
class GameState;

class Engine
{
	CLASS_INSTANCE(Engine);

public:
	 bool Init(vector<string>& _argumentQueue);
	 void Destroy(uint32 _errorCode = 0);

	 bool SetGameState(GameState* _newGameState);

	 bool Tick();

	inline string GetArgument(uint8 index) 
	{ 
		if (arguments.size() > (index + 1))
		{
			return arguments[index + 1];
		}
		else
		{
			return "";
		}
	}
	inline GameState* GetCurrentGameState() const { return currentGameState; }

	inline uint32 GetFPS() const { return framesPerSecond; }

private:
	vector<string> arguments;
	bool needExit;
	GameState* currentGameState;

	uint32 framesCounter, framesPerSecond;
	double framesTimer;

	uint32 t;
	uint32 last_t;
	uint32 _time;

	ConsoleOpenGL* consoleOpenGL;
};

#define _Engine Engine::instance()
