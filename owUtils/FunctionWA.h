#pragma once

#include "Function.h"

/*
    Pointer to function with argument with call ability
*/

template<class ARGTYPE>
class Function_WA : public virtual Function
{
protected:
    typedef void(*procTypeArgs)(ARGTYPE);
    
public:
    Function_WA(procTypeArgs _FuncWAPtr) : Function(nullptr), m_FuncWithArgumentPtr(_FuncWAPtr) {}

protected:  // For Function_SA
    Function_WA() : Function(nullptr), m_FuncWithArgumentPtr(nullptr) {}

public:
    virtual void operator()() override
    {
        fail2("Don't use [operator()] when you call [Function_WA]!");
    }

    virtual void operator()(ARGTYPE _procArgs)
    {
        assert1(m_FuncWithArgumentPtr != nullptr);

        (*m_FuncWithArgumentPtr)(_procArgs);
    }    

protected:
    procTypeArgs m_FuncWithArgumentPtr;
};
#define FUNCTION_WA_Builder(methodName, argType)                   new Function_WA<argType>(&methodName)
#define FUNCTION_STATIC_WA_Builder(className, methodName, argType) new Function_WA<argType>(&className::methodName)

////////////////////////////////////////////////////////////////

template<class T, class ARGTYPE>
class Function_Class_WA : public Function_WA<ARGTYPE>, public virtual Function_Class<T>
{
protected:
	typedef void(T::*classProcTypeArgs)(ARGTYPE);
    
public:
    Function_Class_WA(T* _ClassInstancePtr, classProcTypeArgs _ClassMethodWAPtr) : Function_WA<ARGTYPE>(), Function_Class<T>(_ClassInstancePtr), m_ClassMethodWithArgumentPtr(_ClassMethodWAPtr) {}

protected:
    Function_Class_WA(T* _ClassInstancePtr) : Function_WA<ARGTYPE>(), Function_Class<T>(_ClassInstancePtr), m_ClassMethodWithArgumentPtr(nullptr) {}

public:
    virtual void operator()() override
    {
        fail2("Don't use [operator()] when you call [Function_Class_WA]!");
    }

	virtual void operator()(ARGTYPE _procArgs) override
	{
        assert1(m_ClassInstancePtr != nullptr);
        assert1(m_ClassMethodWithArgumentPtr != nullptr);

		(m_ClassInstancePtr->*m_ClassMethodWithArgumentPtr)(_procArgs);
	}

protected:
    classProcTypeArgs  m_ClassMethodWithArgumentPtr;
};
#define FUNCTION_CLASS_WA_Builder(className, instance, methodName, argType) new Function_Class_WA<className, argType>(instance, &className::methodName)

//

#define FUNCTION_WA_CALL(funcInstance, argType, argValue) \
assert1(dynamic_cast<Function_WA<argType>*>(funcInstance)); \
(dynamic_cast<Function_WA<argType>*>(funcInstance))->operator()(argValue)