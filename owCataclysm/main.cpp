#include "stdafx.h"

// Additional
#include "GameState_Menu.h"

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

	Modules::log().AddDebugOutput(PlatformWindows::CreateDebugOutput_ConsoleWindows());
	//Modules::log().AddDebugOutput(PlatformWindows::CreateDebugOutput_Log());

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
	Modules::log().AddDebugOutput(&openGLConsole);

    //----------------------------------------------------------------

	if (!_Engine->SetGameState(new GameState_Menu))
	{
		_Engine->Destroy(3);
	}

    //----------------------------------------------------------------

	while (_Engine->Tick());

    //----------------------------------------------------------------

	_Engine->Destroy();
}
