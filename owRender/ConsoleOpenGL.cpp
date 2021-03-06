#include "stdafx.h"

// General
#include "ConsoleOpenGL.h"

// Additional
#include "FontsMgr.h"
#include "TexturesMgr.h"
#include "Render.h"

bool ConsoleOpenGL::Init()
{
	windowSize = vec2(_Config.windowSizeX, _Config.windowSizeY);
	opened = false;
	consoleFont = _FontsMgr->GetMainFont();
	assert1(consoleFont != nullptr);
	fontHeight = consoleFont->GetHeight();
	consoleHeight = windowSize.y / 2.0f;
	linesInConsole = consoleHeight / fontHeight;
	textOffset = vec2(5.0f, -(fontHeight + 3.0f));
	lineOffset = 0;
	inputString = "";

	helperSelected = 0;
	helperOffset = vec2(consoleFont->GetStringWidth(">"), 0.0f);

	AddCommonCommands();

    //

    RenderableUIObject::Register(1000);
    InputListenerObject::Register();
    return true;
}

void ConsoleOpenGL::Destroy()
{
	_FontsMgr->Delete(consoleFont);

	RenderableUIObject::Unregister();
    InputListenerObject::Unregister();
}

void ConsoleOpenGL::RenderUI()
{
	if (!opened)
	{
		return;
	}

	// Console rectangle
	_Render->RenderRectangle(VECTOR_ZERO, vec2(windowSize.x, consoleHeight), COLOR_BLACK);

	// Messages
	int curLine = 0;
	for (size_t i = messages.size() - 1 - lineOffset; i > 0; i--)
	{
		auto _message = messages[i];
		if (curLine == linesInConsole)
		{
			break;
		}
		_Render->RenderText(textOffset + vec2(0, consoleHeight - (curLine * fontHeight)), _message->text, consoleFont, _message->color);
		curLine++;
	}

	// Input rectangle
	_Render->RenderRectangle(vec2(0, consoleHeight), vec2(windowSize.x, fontHeight), COLOR_DARKGRAY);

	// Input string
	_Render->RenderText(vec2(0.0f, consoleHeight), ">" + inputString, COLOR_WHITE);

	// Helper
	if (!commandsHelper.empty())
	{
		unsigned i = 1;
		for (auto it = commandsHelper.begin(); it != commandsHelper.end(); ++it)
		{
			auto command = *it;
			auto commandName = command->GetName();
			auto commandNameWidth = consoleFont->GetStringWidth(commandName);

			// Rectangle
			_Render->RenderRectangle(vec2(0.0f, consoleHeight + i * fontHeight), helperOffset + vec2(commandNameWidth, fontHeight), COLOR_GRAY);

			// Selected
			if (helperSelected == i)
			{
				_Render->RenderText(vec2(0.0f, consoleHeight + i * fontHeight), ">", COLOR_WHITE);
				_Render->RenderRectangleOutline(vec2(0.0f, consoleHeight + i * fontHeight), helperOffset + vec2(commandNameWidth, fontHeight), COLOR_BLUE);
			}

			// Text
			string line = commandName;
			if (command->HasArgs() > 0)
			{
				line += " [args]";
			}
			_Render->RenderText(helperOffset + vec2(0.0f, consoleHeight + i * fontHeight), line, COLOR_WHITE);

			i++;
		}
	}
}

//

On_Mouse_WScrolled(ConsoleOpenGL)
{
	if (!opened)
		return false;

	if (messages.size() < linesInConsole)
		return true;

	lineOffset += _yoffset;
	lineOffset = clamp(lineOffset, 0, (int)messages.size() - (int)linesInConsole - 1);

	return true;
}

On_Keyboard_Pressed(ConsoleOpenGL)
{
	if (_key == OW_KEY_GRAVE_ACCENT)
	{
		opened = !opened;
		inputString = "";
		commandsHelper.clear();
		helperSelected = 0;
		return true;
	}

	if (_key == OW_KEY_BACKSPACE && !inputString.empty())
	{
		inputString.pop_back();

		if (!inputString.empty())
		{
			if (inputString.back() != ' ')
				commandsHelper = GetConsoleCommandHelp(inputString);
			else
				commandsHelper.clear();
			helperSelected = 0;
		}

		return true;
	}

	if (_key == OW_KEY_DOWN)
	{
		helperSelected++;
		helperSelected = minf(helperSelected, (int)commandsHelper.size());
	}

	if (_key == OW_KEY_UP)
	{
		helperSelected--;
		helperSelected = maxf(helperSelected, 0);
	}

	if (_key == OW_KEY_ENTER && !inputString.empty())
	{
		int helperSelectedIndex = helperSelected - 1;
		if (helperSelectedIndex != -1 && helperSelectedIndex < commandsHelper.size())
		{
			inputString = commandsHelper[helperSelectedIndex]->GetName() + " ";
			commandsHelper.clear();
			helperSelected = 0;
			return true;
		}

		Log::Print(inputString.c_str());
		Console::ProcessConsoleCommand(inputString);
		inputString = "";
		return true;
	}

	return false;
}

On_Character_Printed(ConsoleOpenGL)
{
    if (!opened)
    {
        return false;
    }

    if (_char == '`')
    {
        return false;
    }

	// If empty line then skip space
    if (inputString.empty() && _char == ' ')
    {
        return true;
    }

	// Skip multiple spaces
	if (!inputString.empty())
	{
		if (inputString.back() == ' ' && _char == ' ')
			return true;
	}

	inputString += _char;

	if (inputString.back() != ' ')
		commandsHelper = GetConsoleCommandHelp(inputString);
	else
		commandsHelper.clear();
	helperSelected = 0;

	return true;
}

//

void ConsoleOpenGL::Print(string _messageFmt, DebugMessageType _type)
{
	Color color = COLOR_WHITE;

	switch (_type)
	{
		case TYPE_INFO:
		color = COLOR_GRAY;
		break;

		case TYPE_PRINT:
		color = COLOR_WHITE;
		break;

		case TYPE_GREEN:
		color = COLOR_GREEN;
		break;

		case TYPE_WARNING:
		color = COLOR_YELLOW;
		break;

		case TYPE_ERROR:
		case TYPE_FATAL:
		color = COLOR_RED;
		break;
	}

	messages.push_back(new ConsoleMessage(_messageFmt, color));
}