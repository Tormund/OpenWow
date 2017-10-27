#include "stdafx.h"

// General
#include "DebugOutput_ConsoleWindows.h"

// Additional
#include <iostream>

bool DebugOutput_ConsoleWindows::Init()
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	defaultConsoleColor = ConsoleWindowsColor::GRAY;

	return true;
}

void DebugOutput_ConsoleWindows::Destroy()
{
}

//

void DebugOutput_ConsoleWindows::Print(string _messageFmt, DebugOutput::DebugMessageType _type)
{
	// Set color
	unsigned short color;
	switch (_type)
	{
		case TYPE_INFO:
		color = ConsoleWindowsColor::GRAY;
		break;
		case TYPE_PRINT:
		color = ConsoleWindowsColor::WHITE;
		break;
		case TYPE_GREEN:
		color = ConsoleWindowsColor::GREEN;
		break;
		case TYPE_WARNING:
		color = ConsoleWindowsColor::YELLOW;
		break;
		case TYPE_ERROR:
		case TYPE_FATAL:
		color = ConsoleWindowsColor::RED;
		break;
	}

	// Add
	SetConsoleTextAttribute(hConsole, color);
	cout << _messageFmt << endl;
	SetConsoleTextAttribute(hConsole, defaultConsoleColor);
}
