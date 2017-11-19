#pragma once

class DayNightPhase
{
public:
	DayNightPhase();
	DayNightPhase(File& f);
	DayNightPhase(DayNightPhase* a, DayNightPhase* b, float r);

	//

	// void setupLighting();
	// void setupFog(); // TODO

    void Render_DEBUG(cvec3 _playerPos);

	float dayIntensity, nightIntensity, ambientIntensity, fogIntensity, fogDepth;
	vec3 dayColor, nightColor, ambientColor, fogColor, dayDir, nightDir;
};