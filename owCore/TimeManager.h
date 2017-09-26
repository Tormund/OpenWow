#pragma once

class TimeManager
{
	CLASS_INSTANCE(TimeManager);

	float animtime;
	int globalTime;
};

#define _TimeManager TimeManager::instance()