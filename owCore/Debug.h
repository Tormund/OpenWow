#pragma once

class DebugOutput;

class Debug {
public:
	static OW_CORE_DLL_API Debug* instance() {
		static Debug instance;
		return &instance;
	}

	//

	static OW_CORE_DLL_API bool Init();
	static OW_CORE_DLL_API void Destroy();

	//

	static OW_CORE_DLL_API bool AddDebugOutput(DebugOutput* _debugOutput);
	static OW_CORE_DLL_API bool DeleteDebugOutput(DebugOutput* _debugOutput);

	//

	static OW_CORE_DLL_API void Info(const char* _message, ...);
	static OW_CORE_DLL_API void Print(const char* _message, ...);
	static OW_CORE_DLL_API void Green(const char* _message, ...);
	static OW_CORE_DLL_API void Warn(const char* _message, ...);
	static OW_CORE_DLL_API void Error(const char* _message, ...);

	//

	static OW_CORE_DLL_API void Fatal(const char* _title, const char* _message, ...);
	static OW_CORE_DLL_API void Exit(int _exitCode);

	//

	static OW_CORE_DLL_API void SetInfoPortion(cstring _infoPortionName);
	static OW_CORE_DLL_API void DeleteInfoPortion(cstring _infoPortionName);
	static OW_CORE_DLL_API bool GetInfoPortion(cstring _infoPortionName);

private:
	static void PushMessageToAllDebugOutputs(const char* _message, int _type, va_list& _vaList);
	static void ShowErrorMessageBox(const char* _message, const char* _title);

private:
	vector<DebugOutput*> debugOutputs;

	vector<string> debugInfoPortions;
};

#include "DebugMacros.h"