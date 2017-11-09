#pragma once

#include "FunctionWA.h"

/*
	Function with args. Keep given arg inside
*/

template<class ARGTYPE>
class Function_SA : public virtual Function_WA<ARGTYPE>
{
public:
	Function_SA(procTypeArgs _FuncWAPtr, ARGTYPE _Argument) : Function_WA<ARGTYPE>(_FuncWAPtr), m_Argument(_Argument) {}
	virtual ~Function_SA()
	{
		//Log::Error("Del arg f");
	}

public:
	void operator()() override
	{
        assert1(m_FuncWithArgumentPtr != nullptr);

		(*m_FuncWithArgumentPtr)(m_Argument);
	}

    void operator()(ARGTYPE _procArgs) override
    {
        fail2("Don't use [operator(ARG)] when you call [Function_SA]!");
    }

    void SetArgument(ARGTYPE _Argument)
    {
        m_Argument = _Argument;
    }

protected:
	ARGTYPE m_Argument;
};
#define FUNCTION_SA_Builder(methodName, argType, arg)                   new Function_SA<argType>(&methodName, arg)
#define FUNCTION_STATIC_SA_Builder(className, methodName, argType, arg) new Function_SA<argType>(&className::methodName, arg)

//////////////////////////////////////////////////////////

template<class T, class ARGTYPE>
class Function_Class_SA : public Function_SA<ARGTYPE>, public virtual Function_Class_WA<T, ARGTYPE>
{
public:
	Function_Class_SA(T* _ClassInstancePtr, classProcTypeArgs _ClassMethodWAPtr, ARGTYPE _arg) : Function_SA<ARGTYPE>(nullptr, _arg), Function_Class_WA<T, ARGTYPE>(_ClassInstancePtr, _ClassMethodWAPtr), Function_Class<T>(_ClassInstancePtr) {}
	~Function_Class_SA() override
	{
		//Log::Error("Del cl f");
	}

	void operator()() override
	{
        assert1(m_ClassInstancePtr != nullptr);
        assert1(m_ClassMethodWithArgumentPtr != nullptr);

		(m_ClassInstancePtr->*m_ClassMethodWithArgumentPtr)(m_Argument);
	}

    void operator()(ARGTYPE _procArgs) override
    {
        fail2("Don't use [operator(ARG)] when you call [Function_Class_SA]!");
    }
};
#define FUNCTION_CLASS_SA_Builder(className, instance, methodName, argType, arg) new Function_Class_SA<className, argType>(instance, &className::methodName, arg)