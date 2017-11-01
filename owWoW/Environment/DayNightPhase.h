#pragma once

class DayNightPhase
{
public:
	DayNightPhase();
	DayNightPhase(File& f);
	DayNightPhase(DayNightPhase* a, DayNightPhase* b, float r);

	//

	void setupLighting();
	// void setupFog(); // TODO

	float dayIntensity, nightIntensity, ambientIntensity, fogIntensity, fogDepth;
	vec3 dayColor, nightColor, ambientColor, fogColor, dayDir, nightDir;

	DirectionalLight m_dirLightDay;
	DirectionalLight m_dirLightNight;
};