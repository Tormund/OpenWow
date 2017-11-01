#pragma once

class PlatformWindows
{
public:
	static OpenGLAdapter* CreateAdapter_GLFW();

	static DebugOutput* CreateDebugOutput_ConsoleWindows();
	static DebugOutput* CreateDebugOutput_Log();
};