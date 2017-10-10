#pragma once

class DebugOutput;

class EngineLog
{
public:
	EngineLog();
	~EngineLog();

	bool AddDebugOutput(DebugOutput* _debugOutput);
	bool DeleteDebugOutput(DebugOutput* _debugOutput);

	void Info(const char* _message, ...);
	void Print(const char* _message, ...);
	void Green(const char* _message, ...);
	void Warn(const char* _message, ...);
	void Error(const char* _message, ...);

	void Fatal(const char* _message, ...);

private:
	void PushMessageToAllDebugOutputs(const char* _message, int _type, va_list& _vaList);

private:
	vector<DebugOutput*> debugOutputs;
	CRITICAL_SECTION debugCS;
};