#pragma once

class ConsoleCommandBase {
public:
	ConsoleCommandBase(cstring _commandName, bool _hasArgs = false) : commandName(Utils::ToLower(_commandName)), hasArgs(_hasArgs) { }

	virtual void Execute() = 0;
	virtual void Execute(cstring _args) = 0;

	// Get functional

	inline const string GetName() const { return commandName; }
	inline bool HasArgs() const { return hasArgs; }

protected:
	string commandName;
	bool hasArgs;
};

//

template <class T>
class ConsoleCommand : public ConsoleCommandBase {
public:
	ConsoleCommand(cstring _commandName, Function<T>* _function, bool _hasArgs = false) : ConsoleCommandBase(_commandName, _hasArgs), function(_function) {	}

	void Execute() override {
		function->operator()();
	}

	void Execute(cstring _args) override {
		T* value = new T;

		if(Utils::TryParse(typeid(T), _args, value))
			function->operator()(T(*(&value[0])));
		else
			Debug::Error("ConsoleCommand[%s]: Can't parse argument [%s] to [%s].", commandName.c_str(), _args.c_str(), typeid(T).name());

		delete value;
	}

private:
	Function<T>* function;
};

//

class Console {
public:
	typedef vector<ConsoleCommandBase*> ConsoleCommands;

public:
	static OW_CORE_DLL_API void AddCommonCommands();

	//

	static OW_CORE_DLL_API bool AddConsoleCommand(ConsoleCommandBase* _command);
	static OW_CORE_DLL_API ConsoleCommandBase* GetConsoleCommandByName(cstring _commandName);

protected:
	static OW_CORE_DLL_API ConsoleCommands GetConsoleCommandHelp(string _input);
	static OW_CORE_DLL_API bool ProcessConsoleCommand(string _line);

private:
	static const unsigned consoleCommandHelpMaxSize = 7;
	static ConsoleCommands consoleCommands;
};

// With args
#define ADDCONSOLECOMMAND_WITHARGS(commandName, func, argType)                         Console::AddConsoleCommand(new ConsoleCommand<argType>(commandName, FUNCTION_WITHARGS(func, argType), true));
#define ADDCONSOLECOMMAND_STATIC_CLASS_WITHARGS(commandName, className, func, argType) Console::AddConsoleCommand(new ConsoleCommand<argType>(commandName, STATIC_FUNCTION_WITHARGS(className, func, argType), true));
#define ADDCONSOLECOMMAND_CLASS_WITHARGS(commandName, className, func, argType)        Console::AddConsoleCommand(new ConsoleCommand<argType>(commandName, CLASS_FUNCTION_WITHARGS(className, this, func, argType), true));

// Without args
#define ADDCONSOLECOMMAND(commandName, func)                                           Console::AddConsoleCommand(new ConsoleCommand<void*>(commandName, FUNCTION(func)));
#define ADDCONSOLECOMMAND_STATIC_CLASS(commandName, className, func)                   Console::AddConsoleCommand(new ConsoleCommand<void*>(commandName, STATIC_FUNCTION(className, func)));
#define ADDCONSOLECOMMAND_CLASS(commandName, className, func)                          Console::AddConsoleCommand(new ConsoleCommand<void*>(commandName, CLASS_FUNCTION(className, this, func)));