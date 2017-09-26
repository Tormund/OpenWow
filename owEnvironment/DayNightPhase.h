#pragma once

class DayNightPhase
{
public:
	void init(File& f);

	void interpolate(DayNightPhase *a, DayNightPhase *b, float r);
	void setupLighting();
	// void setupFog(); // TODO

	//WowTime m_Time;
	int time; // converted from hour:min to the 2880 half-minute ticks thing used in the other Sky thing

	float dayIntensity, nightIntensity, ambientIntensity, fogIntensity, fogDepth;
	vec3 dayColor, nightColor, ambientColor, fogColor, dayDir, nightDir;

	DirectionalLight m_dirLightDay;
	DirectionalLight m_dirLightNight;
};