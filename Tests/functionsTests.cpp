#include "stdafx.h"

Function* fBase0 = nullptr;
Function* fBase1 = nullptr;
Function* fBase2 = nullptr;

Function* fBase3 = nullptr;
Function* fBase4 = nullptr;
Function* fBase5 = nullptr;

Function* fBase6 = nullptr;
Function* fBase7 = nullptr;
Function* fBase8 = nullptr;

class TestClass
{
public:
    void TestClassMethod()
    {
        Log::Error("TestClassMethod Call \n");
    }

    static void TestClassStaticMethod()
    {
        Log::Error("TestClassStaticMethod Call \n");
    }

    //---------

    void TestClassMethodWA(int x)
    {
        Log::Error("TestClassMethod CallWA [%d] \n", x);
    }

    static void TestClassStaticMethodWA(int x)
    {
        Log::Error("TestClassStaticMethodWA [%d] Call \n", x);
    }
};

void TestMethod()
{
    Log::Error("TestMethod Call \n");
}

void TestMethodWA(int x)
{
    Log::Error("TestMethod [%d] Call \n", x);
}

int functionsTests()
{
    TestClass* obj = new TestClass();

    //

    fBase0 = FUNCTION_Builder(TestMethod);
    FUNCTION_CALL(fBase0);

    fBase1 = FUNCTION_CLASS_Builder(TestClass, obj, TestClassMethod);
    FUNCTION_CALL(fBase1);

    fBase2 = FUNCTION_STATIC_Builder(TestClass, TestClassStaticMethod);
    FUNCTION_CALL(fBase2);

    //

    fBase3 = FUNCTION_WA_Builder(TestMethodWA, int);
    FUNCTION_WA_CALL(fBase3, int, 1488);

    fBase4 = FUNCTION_CLASS_WA_Builder(TestClass, obj, TestClassMethodWA, int);
    FUNCTION_WA_CALL(fBase4, int, 1488);

    fBase5 = FUNCTION_STATIC_WA_Builder(TestClass, TestClassStaticMethodWA, int);
    FUNCTION_WA_CALL(fBase5, int, 1488);

    //

    fBase6 = FUNCTION_SA_Builder(TestMethodWA, int, 228);
    fBase6->operator()();

    fBase7 = FUNCTION_CLASS_SA_Builder(TestClass, obj, TestClassMethodWA, int, 228);
    fBase7->operator()();

    fBase8 = FUNCTION_STATIC_SA_Builder(TestClass, TestClassStaticMethodWA, int, 228);
    fBase8->operator()();

    //

    system("pause");
    return 0;
}
