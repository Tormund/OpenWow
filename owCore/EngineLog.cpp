#include "stdafx.h"

// Include
#include "DebugOutput.h"

// General
#include "EngineLog.h"

// Additional
#include "Modules.h"

EngineLog::EngineLog()
{
	InitializeCriticalSection(&debugCS);
}

EngineLog::~EngineLog()
{
	DeleteCriticalSection(&debugCS);

	debugOutputs.clear();
}

// EngineLog outputs functional

bool EngineLog::AddDebugOutput(DebugOutput* _debugOutput)
{
	assert1(_debugOutput != nullptr);

	if (find(debugOutputs.begin(), debugOutputs.end(), _debugOutput) != debugOutputs.end())
	{
		return false;
	}

	if (!_debugOutput->Init())
	{
		EngineLog::Error("Debug[]: Can't init debug output.");
		return false;
	}

	debugOutputs.push_back(_debugOutput);

	return true;
}

bool EngineLog::DeleteDebugOutput(DebugOutput* _debugOutput)
{
	assert1(_debugOutput != nullptr);

	auto _debugOutputsIt = find(debugOutputs.begin(), debugOutputs.end(), _debugOutput);

	// Not exists
	if (_debugOutputsIt == debugOutputs.end())
	{
		return false;
	}

	debugOutputs.erase(_debugOutputsIt);

	return true;
}

// Logs

void EngineLog::Info(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);

	PushMessageToAllDebugOutputs(_message, DebugOutput::DebugMessageType::TYPE_INFO, args);

	va_end(args);
}

void EngineLog::Print(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);

	PushMessageToAllDebugOutputs(_message, DebugOutput::DebugMessageType::TYPE_PRINT, args);

	va_end(args);
}

void EngineLog::Green(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);

	PushMessageToAllDebugOutputs(_message, DebugOutput::DebugMessageType::TYPE_GREEN, args);

	va_end(args);
}

void EngineLog::Warn(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);

	PushMessageToAllDebugOutputs(_message, DebugOutput::DebugMessageType::TYPE_WARNING, args);

	va_end(args);
}

void EngineLog::Error(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);

	PushMessageToAllDebugOutputs(_message, DebugOutput::DebugMessageType::TYPE_ERROR, args);

	va_end(args);
}

// Fatal & exit

void EngineLog::Fatal(const char* _message, ...)
{
	FatalMessageBox(_message, "Fatal");
}

//

void EngineLog::PushMessageToAllDebugOutputs(const char* _message, int _type, va_list& _vaList)
{
	EnterCriticalSection(&debugCS); // THREAD

	if (debugOutputs.empty())
	{
		return;
	}

	for (auto it : debugOutputs)
	{
		it->PushMessage(static_cast<DebugOutput::DebugMessageType>(_type), _message, _vaList);
	}

	LeaveCriticalSection(&debugCS); // THREAD
}

//


