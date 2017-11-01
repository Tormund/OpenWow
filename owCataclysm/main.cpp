#include "stdafx.h"

// Additional
#include "GameState_Menu.h"

int main(int argumentCount, char* arguments[])
{
	_Engine->SetAdapter(PlatformWindows::CreateAdapter_GLFW());

	if (!_Engine->PreInit(argumentCount, arguments))
	{
		_Engine->Destroy(1);
	}

	Modules::log().AddDebugOutput(PlatformWindows::CreateDebugOutput_ConsoleWindows());
	//Modules::log().AddDebugOutput(PlatformWindows::CreateDebugOutput_Log());

	if (!_Engine->Init())
	{
		_Engine->Destroy(2);
	}

	ConsoleOpenGL* openGLConsole = new ConsoleOpenGL();
	openGLConsole->Init();
	Modules::log().AddDebugOutput(openGLConsole);
	Modules::input().AddInputListener(openGLConsole);

	if (!_Engine->SetGameState(new GameState_Menu))
	{
		_Engine->Destroy(3);
	}

	while (_Engine->Tick());

	_Engine->Destroy();
}
