#pragma once

class Console;
class InputListener;
class DebugOutput;

//

struct ConsoleMessage
{
	ConsoleMessage(cstring _text, const Color _color)
	{
		text = _text;
		color = _color;
	}

	string text;
	Color color;
};

//

class ConsoleOpenGL : public Console, public InputListener, public DebugOutput
{
	typedef vector<ConsoleMessage*> ConsoleMessages;
public:
	OW_GAME_DLL_API bool Init();
	OW_GAME_DLL_API void Destroy();

	OW_GAME_DLL_API void RenderUI();

	OW_GAME_DLL_API V_MOUSE_WHEEL;
	OW_GAME_DLL_API V_KEYBD_PRESSED;
	OW_GAME_DLL_API V_CHAR_INPUT;

protected:
	OW_GAME_DLL_API void Print(string _messageFmt, DebugOutput::DebugMessageType _type);

private:
	vec2 windowSize;

	bool opened;
	Font* consoleFont;
	int fontHeight;
	int linesInConsole;
	int consoleHeight;
	vec2 textOffset;

	int lineOffset;

	ConsoleMessages messages;
	string inputString;

	// Help commands
	vector<ConsoleCommandBase*> commandsHelper;
	vec2 helperOffset;
	int helperSelected;
};
