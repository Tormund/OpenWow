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

inline void FatalMessageBox(const char* _title, const char* _message, ...)
{
	va_list args;
	va_start(args, _message);

	int len = vsnprintf(NULL, 0, _message, args);
	char* buff = nullptr;
	if (len > 0)
	{
		buff = new char[len + 1];
		vsnprintf(&buff[0], len + 1, _message, args);
		MessageBoxA(HWND_DESKTOP, buff, _title, MB_ICONERROR | MB_OK);
		delete buff;
	}

	va_end(args);
}

//

#define fail1() \
{ \
	FatalMessageBox("Assertion failed!", "File: [%s]\nLine: [%d]\nFunction: [%s]\n", __FILE__, __LINE__, __FUNCTION__); \
}

#define fail2(message) \
{ \
	FatalMessageBox("Assertion failed!", "File: [%s]\nLine: [%d]\nFunction: [%s]\nDescription: [%s]\n", __FILE__, __LINE__, __FUNCTION__, message); \
}

#define assert1(expr) \
if(!(expr))\
{\
    FatalMessageBox("Assertion failed!", "File: [%s]\nLine: [%d]\nFunction: [%s]\nExpression: [%s]\n", __FILE__, __LINE__, __FUNCTION__, #expr);\
    abort();\
}

#define assert2(expr, message) \
if(!(expr))\
{\
    FatalMessageBox("Assertion failed!", "File: [%s]\nLine: [%d]\nFunction: [%s]\nExpression: [%s]\nDescription: [%s]\n", __FILE__, __LINE__, __FUNCTION__, #expr, message);\
}

#define assert3(expr, message, arg0) \
if(!(expr))\
{\
    FatalMessageBox("Assertion failed!", "File: [%s]\nLine: [%d]\nFunction: [%s]\nExpression: [%s]\nDescription: [%s]\nArgument0: [%s]\n", __FILE__, __LINE__, __FUNCTION__, #expr, message, arg0);\
}

#define assert4(expr, message, arg0, arg1) \
if(!(expr))\
{\
    FatalMessageBox("Assertion failed!", "File: [%s]\nLine: [%d]\nFunction: [%s]\nExpression: [%s]\nDescription: [%s]\nArgument0: [%s]\nArgument1: [%s]\n", __FILE__, __LINE__, __FUNCTION__, #expr, message, arg0, arg1);\
}

#define assert5(expr, message, arg0, arg1, arg2) \
if(!(expr))\
{\
    FatalMessageBox("Assertion failed!", "File: [%s]\nLine: [%d]\nFunction: [%s]\nExpression: [%s]\nDescription: [%s]\nArgument0: [%s]\nArgument1: [%s]\nArgument2: [%s]\n", __FILE__, __LINE__, __FUNCTION__, #expr, message, arg0, arg1, arg2);\
}

#define assert6(expr, message, arg0, arg1, arg2, arg3) \
if(!(expr))\
{\
    FatalMessageBox("Assertion failed!", "File: [%s]\nLine: [%d]\nFunction: [%s]\nExpression: [%s]\nDescription: [%s]\nArgument0: [%s]\nArgument1: [%s]\nArgument2: [%s]\nArgument3: [%s]\n", __FILE__, __LINE__, __FUNCTION__, #expr, message, arg0, arg1, arg2, arg3);\
}