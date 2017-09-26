#pragma once

class ConsoleOpenGL;
class GameState;

class Engine
{
	CLASS_INSTANCE(Engine);

public:
	 bool Init(vector<string>& _argumentQueue);
	 void Destroy(uint32_t _errorCode = 0);

	 bool SetGameState(GameState* _newGameState);

	 bool Tick();

	inline string GetArgument(uint8_t index) 
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

	inline uint32_t GetFPS() const { return framesPerSecond; }

private:
	vector<string> arguments;
	bool needExit;
	GameState* currentGameState;

	uint32_t framesCounter, framesPerSecond;
	double framesTimer;

	uint32_t t;
	uint32_t last_t;
	uint32_t _time;

	ConsoleOpenGL* consoleOpenGL;
};

#define _Engine Engine::instance()
