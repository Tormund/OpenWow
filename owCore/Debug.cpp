#include "stdafx.h"

// Include
#include "DebugOutput.h"

// General
#include "Debug.h"

// Additional
#include <iostream>

bool Debug::Init() {
	return true;
}

void Debug::Destroy() {
	instance()->debugOutputs.clear();
}

// Debug outputs functional

bool Debug::AddDebugOutput(DebugOutput* _debugOutput) {
	if(_debugOutput == nullptr)
		return false;

	auto _debugOutputs = &(instance()->debugOutputs);

	if(find(_debugOutputs->begin(), _debugOutputs->end(), _debugOutput) != _debugOutputs->end())
		return false;

	if(!_debugOutput->Init()) {
		Debug::Error("Can't init debug output.");
		return false;
	}

	_debugOutputs->push_back(_debugOutput);

	return true;
}

bool Debug::DeleteDebugOutput(DebugOutput* _debugOutput) {
	if(_debugOutput == nullptr)
		return false;

	auto _debugOutputs = &(instance()->debugOutputs);
	auto _debugOutputsIt = find(_debugOutputs->begin(), _debugOutputs->end(), _debugOutput);

	// Not exists
	if(_debugOutputsIt == _debugOutputs->end())
		return false;

	// Erase
	_debugOutputs->erase(_debugOutputsIt);

	return true;
}

// Logs

void Debug::Info(const char* _message, ...) {
	va_list args;
	va_start(args, _message);

	PushMessageToAllDebugOutputs(_message, DebugOutput::DebugMessageType::TYPE_INFO, args);

	va_end(args);
}

void Debug::Print(const char* _message, ...) {
	va_list args;
	va_start(args, _message);

	PushMessageToAllDebugOutputs(_message, DebugOutput::DebugMessageType::TYPE_PRINT, args);

	va_end(args);
}

void Debug::Green(const char* _message, ...) {
	va_list args;
	va_start(args, _message);

	PushMessageToAllDebugOutputs(_message, DebugOutput::DebugMessageType::TYPE_GREEN, args);

	va_end(args);
}

void Debug::Warn(const char* _message, ...) {
	va_list args;
	va_start(args, _message);

	PushMessageToAllDebugOutputs(_message, DebugOutput::DebugMessageType::TYPE_WARNING, args);

	va_end(args);
}

void Debug::Error(const char* _message, ...) {
	va_list args;
	va_start(args, _message);

	PushMessageToAllDebugOutputs(_message, DebugOutput::DebugMessageType::TYPE_ERROR, args);

	va_end(args);
}

// Fatal & exit

void Debug::Fatal(const char* _title, const char* _message, ...) {
	va_list args;
	va_start(args, _message);

	int len = vsnprintf(NULL, 0, _message, args);
	char* buff = nullptr;
	if(len > 0) {
		buff = new char[len + 1];
		vsnprintf(&buff[0], len + 1, _message, args);
		ShowErrorMessageBox(buff, _title);
		delete buff;
	}

	va_end(args);

	Debug::Exit(1);
}

void Debug::Exit(int _exitCode) {
	//system("pause");
	exit(_exitCode);
}

//

void Debug::PushMessageToAllDebugOutputs(const char* _message, int _type, va_list& _vaList) {
	auto _debugOutputs = &(instance()->debugOutputs);

	if(_debugOutputs->empty())
		return;

	for(auto it = _debugOutputs->begin(); it != _debugOutputs->end(); ++it)
		(*it)->PushMessage(static_cast<DebugOutput::DebugMessageType>(_type), _message, _vaList);
}

void Debug::ShowErrorMessageBox(const char* _message, const char* _title) {
	/*int msgBoxId = */MessageBoxA(HWND_DESKTOP, _message, _title, MB_ICONERROR | MB_OK);
}

//

void Debug::SetInfoPortion(cstring _infoPortionName) {
	auto _debugInfoPortions = &(instance()->debugInfoPortions);

	// Already exists
	if(find(_debugInfoPortions->begin(), _debugInfoPortions->end(), _infoPortionName) != _debugInfoPortions->end())
		return;

	_debugInfoPortions->push_back(_infoPortionName);
}

void Debug::DeleteInfoPortion(cstring _infoPortionName) {
	auto _debugInfoPortions = &(instance()->debugInfoPortions);
	auto _debugInfoPortionsIt = find(_debugInfoPortions->begin(), _debugInfoPortions->end(), _infoPortionName);

	// Not exists
	if(_debugInfoPortionsIt == _debugInfoPortions->end())
		return;

	// Erase
	_debugInfoPortions->erase(_debugInfoPortionsIt);
}

bool Debug::GetInfoPortion(cstring _infoPortionName) {
	auto _debugInfoPortions = &(instance()->debugInfoPortions);
	auto _debugInfoPortionsIt = find(_debugInfoPortions->begin(), _debugInfoPortions->end(), _infoPortionName);

	// Not exists
	if(_debugInfoPortionsIt == _debugInfoPortions->end())
		return false;

	return true;
}
