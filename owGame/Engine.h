#pragma once

class ConsoleOpenGL;
class GameState;

class Engine {
public:
	static OW_GAME_DLL_API Engine* instance() {
		static Engine instance;
		return &instance;
	}

	//-------------------------------------------------

	OW_GAME_DLL_API bool Init(vector<string>& _argumentQueue);
	OW_GAME_DLL_API void Destroy();

	OW_GAME_DLL_API bool SetGameState(GameState* _newGameState);

	OW_GAME_DLL_API bool Tick();

	inline GameState* GetCurrentGameState() const { return currentGameState; }

	OW_GAME_DLL_API unsigned long long GetTicks() const;
	inline uint32_t GetFPS() const { return framesPerSecond; }

private:
	bool needExit;
	GameState* currentGameState;

	uint32_t framesCounter, framesPerSecond;
	double framesTimer;

	float ftime;
	uint32_t t; 
	uint32_t last_t;
	uint32_t time;

	ConsoleOpenGL* consoleOpenGL;
};

#define _Engine Engine::instance()
