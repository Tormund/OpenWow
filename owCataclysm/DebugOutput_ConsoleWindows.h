#pragma once

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
	 bool Init();
	 void Destroy();

protected:
	 void Print(string _messageFmt, DebugOutput::DebugMessageType _type);

private:
	HANDLE hConsole;
	unsigned short defaultConsoleColor;
};
