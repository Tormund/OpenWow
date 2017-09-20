#pragma once

#include "Debug.h"
#include "DebugMacros.h"

/*
	Pointer to function, that take arg
*/

template<class ARGTYPE>
class Function
{
	typedef void(*procType)();
	typedef void(*procTypeArgs)(ARGTYPE);

public:
	Function() : proc(nullptr), procArgs(nullptr) {}
	Function(procType _proc) : proc(_proc), procArgs(nullptr) {}
	Function(procTypeArgs _procArgs) : proc(nullptr), procArgs(_procArgs) {}

	virtual void operator()()
	{
		if (proc != nullptr)
		{
			(*proc)();
		}
		else
		{
			fail1();
		}
	}

	virtual void operator()(ARGTYPE _procArgs)
	{
		if (proc != nullptr)
		{
			(*procArgs)(_procArgs);
		}
		else
		{
			fail1();
		}
	}

private:
	procType proc;
	procTypeArgs procArgs;
};

#define FUNCTION_WITHARGS(methodName, argType)                   new Function<argType>(&methodName)
#define FUNCTION(methodName)                                     new Function<void*>  (&methodName)

#define STATIC_FUNCTION_WITHARGS(className, methodName, argType) new Function<argType>(&className::methodName)
#define STATIC_FUNCTION(className, methodName)                   new Function<void*>  (&className::methodName)

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

template<class T, class ARGTYPE>
class ClassFunction final : public virtual Function<ARGTYPE>
{
	typedef void(T::*classProcType)();
	typedef void(T::*classProcTypeArgs)(ARGTYPE);

public:
	ClassFunction(T* _instance, classProcType _classProc) : classProc(_classProc), classProcArgs(nullptr), instance(_instance) {}
	ClassFunction(T* _instance, classProcTypeArgs _classProcArgs) : classProc(nullptr), classProcArgs(_classProcArgs), instance(_instance) {}

	void operator()() override
	{
		if (instance != nullptr && classProc != nullptr)
		{
			(instance->*classProc)();
		}
		else
		{
			fail1();
		}
	}

	void operator()(ARGTYPE _procArgs) override
	{
		if (instance != nullptr && classProcArgs != nullptr && _procArgs != nullptr)
		{
			(instance->*classProcArgs)(_procArgs);
		}
		else
		{
			fail1();
		}
	}

private:
	T* instance;
	classProcType classProc;
	classProcTypeArgs classProcArgs;
};

#define CLASS_FUNCTION_WITHARGS(className, instance, methodName, argType) new ClassFunction<className, argType>(instance, &className::methodName)
#define CLASS_FUNCTION(className, instance, methodName)                   new ClassFunction<className, void*>  (instance, &className::methodName)

////////////////////////////////////////////////////////////////

/*
	Function with args. Keep given arg inside
*/


class ArgFunctionBase
{
public:
	virtual ~ArgFunctionBase()
	{
		Debug::Error("Del base f");
	}
	virtual void operator()() = 0;
};

//////////////////////////////////////////////////////////

template<class ARGTYPE>
class ArgFunction : public virtual ArgFunctionBase
{
	typedef void(*procTypeArgs)(ARGTYPE);

public:
	ArgFunction(procTypeArgs _procArgs, ARGTYPE& _arg) : procArgs(_procArgs), arg(_arg) {}
	virtual ~ArgFunction() override
	{
		Debug::Error("Del arg f");
	}
	
protected:
	ArgFunction(ARGTYPE& _arg) : procArgs(nullptr), arg(_arg) {}

public:
	virtual void operator()() override
	{
		if (procArgs != nullptr && arg != nullptr)
		{
			(*procArgs)(arg);
		}
		else
		{
			fail1();
		}
	}

protected:
	ARGTYPE arg;

private:
	procTypeArgs procArgs;
};

#define ARG_FUNCTION(methodName, argType, arg)                   new ArgFunction<argType>(&methodName, arg)
#define STATIC_ARG_FUNCTION(className, methodName, argType, arg) new ArgFunction<argType>(&className::methodName, arg)

//////////////////////////////////////////////////////////

template<class T, class ARGTYPE>
class ClassArgFunction final : public virtual ArgFunction<ARGTYPE>
{
	typedef void(T::*classProcTypeArgs)(ARGTYPE);

public:
	ClassArgFunction(T* _instance, classProcTypeArgs _classProcArgs, ARGTYPE& _arg) : instance(_instance), classProcArgs(_classProcArgs), ArgFunction<ARGTYPE>(_arg) {}
	~ClassArgFunction() override
	{
		Debug::Error("Del cl f");
	}

	virtual void operator()() override
	{
		if (instance != nullptr && classProcArgs != nullptr && arg != nullptr)
		{
			(instance->*classProcArgs)(arg);
		}
		else
		{
			fail1();
		}
	}

private:
	T* instance;
	classProcTypeArgs classProcArgs;
};
#define CLASS_ARG_FUNCTION(className, instance, methodName, argType, arg) new ClassArgFunction<className, argType>(instance, &className::methodName, arg)