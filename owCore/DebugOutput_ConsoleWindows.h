#pragma once

#include "DebugOutput.h"

class DebugOutput_ConsoleWindows : public DebugOutput
{
	enum ConsoleWindowsColor
	{
		GRAY = 8,
		BLUE = 9,
		GREEN = 10,
		LIGHTBLUE = 11,
		RED = 12,
		PINK = 13,
		YELLOW = 14,
		WHITE = 15
	};

public:
	OW_CORE_DLL_API bool Init();
	OW_CORE_DLL_API void Destroy();

protected:
	OW_CORE_DLL_API void Print(string _messageFmt, DebugOutput::DebugMessageType _type);

private:
	HANDLE hConsole;
	unsigned short defaultConsoleColor;
};
