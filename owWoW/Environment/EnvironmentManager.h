#pragma once

#include "DayNightCycle.h"

#include "MapSkies.h"



//

class EnvironmentManager : public Module
{
	DEF_MODULE(EnvironmentManager);

	void InitSkies(DBC_MapRecord* _mapRecord);

	//

	void outdoorLighting();
	void SetAmbientLights(bool on);
	void SetFog();

	//

	vec3 GetSkyColor(SkyColorNames _color)
	{
		return skies->colorSet[_color];
	}

public:
	void BeforeDraw();

public:
	vec4 m_OutdoorAmbientColor;
	vec4 m_OutdoorDayDiffuseColor;
	vec4 m_OutdoorNightDiffuseColor;
	vec4 m_OutdoorSpecularColor;
	

public:
	float animtime;
	int globalTime;


	bool m_HasSky;
	WowTime m_GameTime;


	MapSkies* skies;

	DayNightCycle* dayNightCycle;
	DayNightPhase dayNightPhase;
};

#define _EnvironmentManager EnvironmentManager::instance()