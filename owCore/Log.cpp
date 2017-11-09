#include "stdafx.h"

// Include
#include "DebugOutput.h"

// General
#include "Log.h"

vector<DebugOutput*> Log::debugOutputs;
CRITICAL_SECTION     Log::debugCS;

bool Log::Init()
{
	InitializeCriticalSection(&debugCS);

    return true;
}

void Log::Destroy()
{
	DeleteCriticalSection(&debugCS);

	debugOutputs.clear();
}

// Log outputs functional

bool Log::AddDebugOutput(DebugOutput* _debugOutput)
{
	assert1(_debugOutput != nullptr);

	if (find(debugOutputs.begin(), debugOutputs.end(), _debugOutput) != debugOutputs.end())
	{
		return false;
	}

	if (!_debugOutput->Init())
	{
		Log::Error("Debug[]: Can't init debug output.");
		return false;
	}

	debugOutputs.push_back(_debugOutput);

	return true;
}

bool Log::DeleteDebugOutput(DebugOutput* _debugOutput)
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

void Log::Info(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);

	PushMessageToAllDebugOutputs(_message, DebugOutput::DebugMessageType::TYPE_INFO, args);

	va_end(args);
}

void Log::Print(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);

	PushMessageToAllDebugOutputs(_message, DebugOutput::DebugMessageType::TYPE_PRINT, args);

	va_end(args);
}

void Log::Green(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);

	PushMessageToAllDebugOutputs(_message, DebugOutput::DebugMessageType::TYPE_GREEN, args);

	va_end(args);
}

void Log::Warn(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);

	PushMessageToAllDebugOutputs(_message, DebugOutput::DebugMessageType::TYPE_WARNING, args);

	va_end(args);
}

void Log::Error(const char* _message, ...)
{
	va_list args;
	va_start(args, _message);

	PushMessageToAllDebugOutputs(_message, DebugOutput::DebugMessageType::TYPE_ERROR, args);

	va_end(args);
}

// Fatal & exit

void Log::Fatal(const char* _message, ...)
{
	FatalMessageBox(_message, "Fatal");
}

//

void Log::PushMessageToAllDebugOutputs(const char* _message, int _type, va_list& _vaList)
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


