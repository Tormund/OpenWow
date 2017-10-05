#pragma once

namespace StaticAssert
{
	template<bool>
	struct StaticAssertFailed;

	template<>
	struct StaticAssertFailed<true> {};
}
#define assert_static(exp) (StaticAssert::StaticAssertFailed<(exp) != 0>())

//

#define assert1(expr) \
if(!(expr))\
{\
Debug::Fatal("Assertion failed!", "File: [%s]\nLine: [%d]\nFunction: [%s]\nExpression: [%s]\n", __FILE__, __LINE__, __FUNCTION__, #expr);\
}

#define assert2(expr, message) \
if(!(expr))\
{\
Debug::Fatal("Assertion failed!", "File: [%s]\nLine: [%d]\nFunction: [%s]\nExpression: [%s]\nDescription: [%s]\n", __FILE__, __LINE__, __FUNCTION__, #expr, message);\
}

#define assert3(expr, message, arg0) \
if(!(expr))\
{\
Debug::Fatal("Assertion failed!", "File: [%s]\nLine: [%d]\nFunction: [%s]\nExpression: [%s]\nDescription: [%s]\nArgument0: [%s]\n", __FILE__, __LINE__, __FUNCTION__, #expr, message, arg0);\
}

#define assert4(expr, message, arg0, arg1) \
if(!(expr))\
{\
Debug::Fatal("Assertion failed!", "File: [%s]\nLine: [%d]\nFunction: [%s]\nExpression: [%s]\nDescription: [%s]\nArgument0: [%s]\nArgument1: [%s]\n", __FILE__, __LINE__, __FUNCTION__, #expr, message, arg0, arg1);\
}

#define assert5(expr, message, arg0, arg1, arg2) \
if(!(expr))\
{\
Debug::Fatal("Assertion failed!", "File: [%s]\nLine: [%d]\nFunction: [%s]\nExpression: [%s]\nDescription: [%s]\nArgument0: [%s]\nArgument1: [%s]\nArgument2: [%s]\n", __FILE__, __LINE__, __FUNCTION__, #expr, message, arg0, arg1, arg2);\
}

#define assert6(expr, message, arg0, arg1, arg2, arg3) \
if(!(expr))\
{\
Debug::Fatal("Assertion failed!", "File: [%s]\nLine: [%d]\nFunction: [%s]\nExpression: [%s]\nDescription: [%s]\nArgument0: [%s]\nArgument1: [%s]\nArgument2: [%s]\nArgument3: [%s]\n", __FILE__, __LINE__, __FUNCTION__, #expr, message, arg0, arg1, arg2, arg3);\
}

#define fail1() Debug::Fatal("Assertion failed!", "File: [%s]\nLine: [%d]\nFunction: [%s]\n", __FILE__, __LINE__, __FUNCTION__);