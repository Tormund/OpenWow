#pragma once

class DebugOutput;

class Debug
{
	CLASS_INSTANCE(Debug);

	static  bool Init();
	static  void Destroy();

	//

	static  bool AddDebugOutput(DebugOutput* _debugOutput);
	static  bool DeleteDebugOutput(DebugOutput* _debugOutput);

	//

	static  void Info(const char* _message, ...);
	static  void Print(const char* _message, ...);
	static  void Green(const char* _message, ...);
	static  void Warn(const char* _message, ...);
	static  void Error(const char* _message, ...);

	//

	static  void Fatal(const char* _title, const char* _message, ...);
	static  void Exit(int _exitCode);

private:
	static void PushMessageToAllDebugOutputs(const char* _message, int _type, va_list& _vaList);
	static void ShowErrorMessageBox(const char* _message, const char* _title);

private:
	vector<DebugOutput*> debugOutputs;
	static CRITICAL_SECTION debugCS;
};

#include "DebugMacros.h"