#pragma once

#include "DayNightCycle.h"

#include "MapSkies.h"

// Consts
const uint32_t C_Game_SecondsInMinute = 2;
const uint32_t C_Game_MinutesInHour = 60;
const uint32_t C_Game_HoursInDay = 24;

const uint32_t C_Game_SecondsInHour = (C_Game_SecondsInMinute * C_Game_MinutesInHour);
const uint32_t C_Game_SecondsInDay = (C_Game_SecondsInMinute * C_Game_MinutesInHour * C_Game_HoursInDay);

//

class EnvironmentManager : public Module
{
	DEF_MODULE(EnvironmentManager);

	// Time functional
	void Time_SetGameTime(uint32_t _hour, uint32_t _minute);
	inline uint32_t Time_GetCurrentDay() const { return m_GameTime / C_Game_SecondsInDay; }
	inline uint32_t Time_GetTime() const { return m_GameTime % C_Game_SecondsInDay; }
	inline uint32_t Time_GetHour() const { return (m_GameTime % C_Game_SecondsInDay) / C_Game_SecondsInHour; }
	inline uint32_t Time_GetMinute() const { return (m_GameTime % C_Game_SecondsInDay) % C_Game_SecondsInHour; }


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
	bool m_HasSky;
	uint32_t m_GameTime; // In game seconds


	MapSkies* skies;

	DayNightCycle* dayNightCycle;
	DayNightPhase dayNightPhase;
};

#define _EnvironmentManager EnvironmentManager::instance()