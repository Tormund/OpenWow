#pragma once

#include "DayNightPhase.h"

class DayNightCycle
{
public:
	DayNightCycle();

	DayNightPhase getPhase(int time);

private:
	vector<DayNightPhase> dayNightPhases;
};
