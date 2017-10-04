#pragma once

#include "DayNightCycle.h"

#include "MapSkies.h"



//

class EnvironmentManager : public Module
{
	DEF_MODULE(EnvironmentManager);

	void InitSkies(uint32_t mapid);

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
	bool m_HasSky;
	WowTime m_GameTime;


	MapSkies* skies;

	DayNightCycle* dayNightCycle;
	DayNightPhase dayNightPhase;
};

#define _EnvironmentManager EnvironmentManager::instance()