#include "stdafx.h"

#include <cstdio>

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
        printf("TestClassMethod Call \n");
    }

    static void TestClassStaticMethod()
    {
        printf("TestClassStaticMethod Call \n");
    }

    //---------

    void TestClassMethodWA(int x)
    {
        printf("TestClassMethod CallWA [%d] \n", x);
    }

    static void TestClassStaticMethodWA(int x)
    {
        printf("TestClassStaticMethodWA [%d] Call \n", x);
    }
};

void TestMethod()
{
    printf("TestMethod Call \n");
}

void TestMethodWA(int x)
{
    printf("TestMethod [%d] Call \n", x);
}

int main(int argumentCount, char* arguments[])
{
    TestClass* obj = new TestClass();

    //

    fBase0 = FUNCTION_Builder(TestMethod);
    fBase0->operator()();

    fBase1 = FUNCTION_CLASS_Builder(TestClass, obj, TestClassMethod);
    fBase1->operator()();

    fBase2 = FUNCTION_STATIC_Builder(TestClass, TestClassStaticMethod);
    fBase2->operator()();

    //

    fBase3 = FUNCTION_WA_Builder(TestMethodWA, int);
    (dynamic_cast<Function_WA<int>*>(fBase3))->operator()(1488);

    fBase4 = FUNCTION_CLASS_WA_Builder(TestClass, obj, TestClassMethodWA, int);
    (dynamic_cast<Function_WA<int>*>(fBase4))->operator()(1488);

    fBase5 = FUNCTION_STATIC_WA_Builder(TestClass, TestClassStaticMethodWA, int);
    (dynamic_cast<Function_WA<int>*>(fBase5))->operator()(1488);

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
