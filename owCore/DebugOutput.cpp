#include "stdafx.h"

// General
#include "DebugOutput.h"

void DebugOutput::PushMessage(DebugOutput::DebugMessageType _type, const char* _message, va_list& _vaList)
{
	// Parse args end
	int len = vsnprintf(NULL, 0, _message, _vaList);
	string buff;
	if (len > 0)
	{
		buff.resize(len + 1);
		vsnprintf(&buff[0], len + 1, _message, _vaList);
		PushMessage(buff, _type);
	}
}

void DebugOutput::PushMessage(cstring _message, DebugOutput::DebugMessageType _type)
{
	string messageWithPrefix;

	// Set Prefix
	switch (_type)
	{
		case TYPE_INFO:
		messageWithPrefix = "~" + string(_message);
		break;
		case TYPE_PRINT:
		messageWithPrefix = " " + string(_message);
		break;
		case TYPE_GREEN:
		messageWithPrefix = "^" + string(_message);
		break;
		case TYPE_WARNING:
		messageWithPrefix = "@" + string(_message);
		break;
		case TYPE_ERROR:
		messageWithPrefix = "!" + string(_message);
		break;
		case TYPE_FATAL:
		messageWithPrefix = "FATAL ERROR: " + string(_message);
		break;
	}

	Print(messageWithPrefix, _type);
}
