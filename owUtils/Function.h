#pragma once

/*
    Pointer to function with call ability
*/

class Function
{
protected:
	typedef void(*procType)();

public:
	Function(procType _FuncPtr) : m_FuncPtr(_FuncPtr) {}

protected:
    Function() : m_FuncPtr(nullptr) {}

public:
	virtual void operator()()
	{
        assert1(m_FuncPtr != nullptr);

		(*m_FuncPtr)();
	}

protected:
	procType m_FuncPtr;
};
#define FUNCTION_Builder(methodName)                   new Function(&methodName)
#define FUNCTION_STATIC_Builder(className, methodName) new Function(&className::methodName)

////////////////////////////////////////////////////////////////

template<class T>
class Function_Class : public virtual Function
{
protected:
	typedef void(T::*classProcType)();

public:
	Function_Class(T* _ClassInstancePtr, classProcType _ClassMethodPtr) : Function(), m_ClassInstancePtr(_ClassInstancePtr), m_ClassMethodPtr(_ClassMethodPtr) {}

protected:
    Function_Class(T* _ClassInstancePtr) : Function(), m_ClassInstancePtr(_ClassInstancePtr), m_ClassMethodPtr(nullptr) {}

public:
	virtual void operator()() override
	{
        assert1(m_ClassInstancePtr != nullptr);
        assert1(m_ClassMethodPtr != nullptr);

		(m_ClassInstancePtr->*m_ClassMethodPtr)();
	}

protected:
	T*             m_ClassInstancePtr;
	classProcType  m_ClassMethodPtr;
};
#define FUNCTION_CLASS_Builder(className, instance, methodName) new Function_Class<className>  (instance, &className::methodName)

//

#define FUNCTION_CALL(funcInstance) funcInstance->operator()()