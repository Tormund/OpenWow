#pragma once

class DebugOutput;

class Log
{
public:
    static bool Init();
    static void Destroy();

	static bool AddDebugOutput(DebugOutput* _debugOutput);
    static bool DeleteDebugOutput(DebugOutput* _debugOutput);

    static void Info(const char* _message, ...);
    static void Print(const char* _message, ...);
    static void Green(const char* _message, ...);
    static void Warn(const char* _message, ...);
    static void Error(const char* _message, ...);

    static void Fatal(const char* _message, ...);

private:
    static void PushMessageToAllDebugOutputs(const char* _message, int _type, va_list& _vaList);

private:
    static vector<DebugOutput*> debugOutputs;
    static CRITICAL_SECTION debugCS;
};