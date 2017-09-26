#include "stdafx.h"

// General
#include "DayNightCycle.h"

DayNightCycle::DayNightCycle()
{
	File f("World\\dnc.db");
	if (!f.Open())
	{
		Debug::Error("DayNightCycle[]: Can't init day-night cycle.");
		return;
	}

	uint32_t nFields1, nFields, d;

	// Header
	f.ReadBytes(&nFields1, 4);
	f.ReadBytes(&nFields, 4);
	assert2(nFields == nFields1, "DayNightCycle: nFields mismatch!");
	Debug::Info("DayNightCycle nFields = [%d]", nFields);

	// Field Descriptions
	uint32_t magic0x53;
	f.ReadBytes(&magic0x53, 4);
	assert2(magic0x53 == 0x53, "DayNightCycle: magic0x53 is not 0x53!");
	f.ReadBytes(&d, 4); // d is now the final offset

	// Skip names
	f.Seek(8 + nFields * 8);

	while (f.GetPos() < d)
	{
		DayNightPhase ols;
		ols.init(f);

		dayNightPhases.push_back(ols);
	}
}

DayNightPhase DayNightCycle::getPhase(int time)
{
	int ta = time / 120;
	int tb = (ta + 1) % 24;

	float r = (time - (ta * 120)) / 120.0f;

	DayNightPhase* a = &dayNightPhases[ta];
	DayNightPhase* b = &dayNightPhases[tb];

	DayNightPhase out;
	out.interpolate(a, b, r);

	return out;
}