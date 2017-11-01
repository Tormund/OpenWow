#include "stdafx.h"

// General
#include "PlatformWindows.h"

// Additiobal
#include "DebugOutput_ConsoleWindows.h"
#include "DebugOutput_Log.h"
#include "OpenGLAdapter_GLFW.h"

DebugOutput* PlatformWindows::CreateDebugOutput_ConsoleWindows()
{
	return new DebugOutput_ConsoleWindows();
}

DebugOutput* PlatformWindows::CreateDebugOutput_Log()
{
	return new DebugOutput_Log();
}

OpenGLAdapter* PlatformWindows::CreateAdapter_GLFW()
{
	return new OpenGLAdapter_GLFW();
}
