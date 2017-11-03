#pragma once

class ConsoleCommand
{
public:
	ConsoleCommand(cstring _commandName, Function* _function) : m_Name(Utils::ToLower(_commandName)), m_Function(_function), m_HasArgs(false) {}

    //

    void Execute()
    {
        m_Function->operator()();
    }

    virtual void Execute(cstring _args)
    {
        fail2("Unable to call ConsoleCommand::Execute(cstring _args) in base class!");
    }

	// Get functional

	const string GetName() const { return m_Name; }
	bool HasArgs() const { return m_HasArgs; }

protected:
	string     m_Name;
	bool       m_HasArgs;
    Function*  m_Function;
};

template <class ARGTYPE>
class ConsoleCommand_WA : public ConsoleCommand
{
public:
	ConsoleCommand_WA(cstring _commandName, Function* _function) : ConsoleCommand(_commandName, _function), m_HasArgs(true) {}

    //

	void Execute(cstring _args) override
	{
		ARGTYPE value;

		if (Utils::TryParse(typeid(ARGTYPE), _args, &value))
		{
            Function_WA<ARGTYPE>* funcWA = dynamic_cast<Function_WA<ARGTYPE>*>(m_Function);
            assert1(funcWA != nullptr);

            funcWA->operator()(ARGTYPE(value));
		}
		else
		{
			Modules::log().Error("ConsoleCommand_WA[%s]: Can't parse argument [%s] to [%s].", m_Name.c_str(), _args.c_str(), typeid(ARGTYPE).name());
		}
	}
};

//

class Console
{
public:
	typedef vector<ConsoleCommand*> ConsoleCommands;

public:
	static void AddCommonCommands();

	//

	static bool AddConsoleCommand(ConsoleCommand* _command);
	static ConsoleCommand* GetConsoleCommandByName(cstring _commandName);

protected:
	static ConsoleCommands GetConsoleCommandHelp(string _input);
	static bool ProcessConsoleCommand(string _line);

private:
	static const unsigned consoleCommandHelpMaxSize = 7;
	static ConsoleCommands consoleCommands;
};

// With args
#define ADDCONSOLECOMMAND_WITHARGS(commandName, func, argType)                      Console::AddConsoleCommand(new ConsoleCommand_WA<argType>(commandName, FUNCTION_WA_Builder(func, argType)));
#define ADDCONSOLECOMMAND_STATIC_WITHARGS(commandName, className, func, argType)    Console::AddConsoleCommand(new ConsoleCommand_WA<argType>(commandName, FUNCTION_STATIC_WA_Builder(className, func, argType)));
#define ADDCONSOLECOMMAND_CLASS_WITHARGS(commandName, className, func, argType)     Console::AddConsoleCommand(new ConsoleCommand_WA<argType>(commandName, FUNCTION_CLASS_WA_Builder(className, this, func, argType)));

// Without args
#define ADDCONSOLECOMMAND(commandName, func)                    Console::AddConsoleCommand(new ConsoleCommand(commandName, FUNCTION_Builder(func)));
#define ADDCONSOLECOMMAND_STATIÑ(commandName, className, func)  Console::AddConsoleCommand(new ConsoleCommand(commandName, FUNCTION_STATIC_Builder(className, func)));
#define ADDCONSOLECOMMAND_CLASS(commandName, className, func)   Console::AddConsoleCommand(new ConsoleCommand(commandName, FUNCTION_CLASS_Builder(className, this, func)));