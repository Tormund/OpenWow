#pragma once

#include "DayNightPhase.h"

class DayNightCycle
{
public:
	DayNightCycle();

	DayNightPhase getPhase(uint32_t _gameTime);

private:
	vector<DayNightPhase> dayNightPhases;
};
