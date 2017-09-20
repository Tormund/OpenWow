#pragma once

#include "DebugOutput.h"

class DebugOutput_Log : public DebugOutput {
public:
	OW_CORE_DLL_API bool Init();
	OW_CORE_DLL_API void Destroy();

protected:
	OW_CORE_DLL_API void Print(string _messageFmt, DebugOutput::DebugMessageType _type);

private:
	ofstream logStream;
};
