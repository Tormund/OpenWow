#include "stdafx.h"

// Additional
#include "GameState_Menu.h"
#include "GameState_InWorld.h"

int main(int argumentCount, char* arguments[])
{
    // Configure adapter
	_Engine->SetAdapter(PlatformWindows::CreateAdapter_GLFW());

    //----------------------------------------------------------------

    // Pre init engine
	if (!_Engine->PreInit(argumentCount, arguments))
	{
		_Engine->Destroy(1);
	}

    //----------------------------------------------------------------

	Log::AddDebugOutput(PlatformWindows::CreateDebugOutput_ConsoleWindows());
	//Log::AddDebugOutput(PlatformWindows::CreateDebugOutput_Log());

    _UIMgr->Init();

    //----------------------------------------------------------------

    // After add debug outputs start engine
	if (!_Engine->Init())
	{
		_Engine->Destroy(2);
	}

    //----------------------------------------------------------------

    // Custom debug outputs
	ConsoleOpenGL openGLConsole;
	openGLConsole.Init();
	Log::AddDebugOutput(&openGLConsole);

    //----------------------------------------------------------------

    GameStateManager::AddGameState(GameStatesNames::GAME_STATE_MENU, new GameState_Menu);
    GameStateManager::AddGameState(GameStatesNames::GAME_STATE_WORLD, new GameState_InWorld);

    GameStateManager::SetGameState(GameStatesNames::GAME_STATE_MENU);

    //----------------------------------------------------------------

	while (_Engine->Tick());

    //----------------------------------------------------------------

	_Engine->Destroy();
}
