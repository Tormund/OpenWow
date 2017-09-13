#pragma once

class ConsoleOpenGL;
class GameState;

class Engine
{
	CLASS_INSTANCE_API(Engine, OW_GAME_DLL_API);

public:
	OW_GAME_DLL_API bool Init(vector<string>& _argumentQueue);
	OW_GAME_DLL_API void Destroy(uint32_t _errorCode = 0);

	OW_GAME_DLL_API bool SetGameState(GameState* _newGameState);

	OW_GAME_DLL_API bool Tick();

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

	OW_GAME_DLL_API unsigned long long GetTicks() const;
	inline uint32_t GetFPS() const { return framesPerSecond; }

private:
	vector<string> arguments;
	bool needExit;
	GameState* currentGameState;

	uint32_t framesCounter, framesPerSecond;
	double framesTimer;

	float ftime;
	uint32_t t;
	uint32_t last_t;
	uint32_t _time;

	ConsoleOpenGL* consoleOpenGL;
};

#define _Engine Engine::instance()
