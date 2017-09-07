#pragma once

class DayNightPhase
{
public:
	void init(File& f);

	void interpolate(DayNightPhase *a, DayNightPhase *b, float r);
	void setupLighting();
	// void setupFog(); // TODO


	int time; // converted from hour:min to the 2880 half-minute ticks thing used in the other Sky thing

	float dayIntensity, nightIntensity, ambientIntensity, fogIntensity, fogDepth;
	vec3 dayColor, nightColor, ambientColor, fogColor, dayDir, nightDir;
};

class DayNightCycle
{
public:
	DayNightCycle();

	DayNightPhase getPhase(int time);

private:
	vector<DayNightPhase> dayNightPhases;
};
