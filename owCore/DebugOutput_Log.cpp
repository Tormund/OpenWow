#include "stdafx.h"

// General
#include "DebugOutput_Log.h"

#define LOGFILENAME "log.txt"

bool DebugOutput_Log::Init()
{
	fileName = string(LOGFILENAME);

	if (fileName.empty())
	{
		Debug::Error("LogFile: No such file [%s] or directory!", fileName.c_str());
		return false;
	}

	logStream.open(fileName.c_str(), ios::out);
	if (!logStream.is_open())
	{
		Debug::Error("LogFile: Can not open file [%s]!", fileName.c_str());
		logStream.clear();
		return false;
	}

	return true;
}

void DebugOutput_Log::Destroy()
{
	logStream.flush();
	logStream.close();
	logStream.clear();
}

//

void DebugOutput_Log::Print(string _messageFmt, DebugOutput::DebugMessageType _type)
{
	logStream << _messageFmt << endl;
	logStream.flush();
}
