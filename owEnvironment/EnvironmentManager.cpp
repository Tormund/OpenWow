#include "stdafx.h"

// General
#include "EnvironmentManager.h"


bool EnvironmentManager::Init()
{
	Time_SetGameTime(12, 45);

	skies = 0;
	dayNightCycle = new DayNightCycle();

	
	return true;
}

void EnvironmentManager::Destroy()
{
	if (skies)
	{
		delete skies;
	}

	if (dayNightCycle)
	{
		delete dayNightCycle;
	}
}



void EnvironmentManager::Time_SetGameTime(uint32_t _hour, uint32_t _minute)
{
	m_GameTime = _hour * C_Game_SecondsInHour + _minute * C_Game_SecondsInMinute;
}

void EnvironmentManager::InitSkies(uint32_t mapid)
{
	skies = new MapSkies(mapid);
}





void EnvironmentManager::outdoorLighting()
{
	vec4 black(0, 0, 0, 0);
	vec4 ambient(skies->colorSet[LIGHT_GLOBAL_AMBIENT], 1);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, glm::value_ptr(ambient));


	// Day diffuse
	vec4 posDay(0, 0, 0, 0);
	vec4 colorDay(skies->colorSet[LIGHT_GLOBAL_DIFFUSE] * dayNightPhase.dayIntensity, 1.0f);
	glLightfv(GL_LIGHT0, GL_AMBIENT, glm::value_ptr(black));
	glLightfv(GL_LIGHT0, GL_DIFFUSE, glm::value_ptr(colorDay));
	glLightfv(GL_LIGHT0, GL_POSITION, glm::value_ptr(posDay));

	const float spc = _Settings->useshaders ? 1.4f : 0.0f;
	vec4 spcol(spc, spc, spc, 1.0f);
	glLightfv(GL_LIGHT0, GL_SPECULAR, glm::value_ptr(spcol));


	// Night diffuse
	vec4 posNight(0, 0, 0, 0);
	vec4 colorNight(skies->colorSet[LIGHT_GLOBAL_DIFFUSE] * dayNightPhase.nightIntensity, 1.0f);
	glLightfv(GL_LIGHT1, GL_AMBIENT, glm::value_ptr(black));
	glLightfv(GL_LIGHT1, GL_DIFFUSE, glm::value_ptr(colorNight));
	glLightfv(GL_LIGHT1, GL_POSITION, glm::value_ptr(posNight));

	const float spcNight = _Settings->useshaders ? 1.4f : 0.0f;
	vec4 spcolorNight(spcNight, spcNight, spcNight, 1.0f);
	glLightfv(GL_LIGHT1, GL_SPECULAR, glm::value_ptr(spcolorNight));
}



void EnvironmentManager::SetAmbientLights(bool on)
{
	if (on)
	{
		vec4 ambient(skies->colorSet[LIGHT_GLOBAL_AMBIENT], 1);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, glm::value_ptr(ambient));

		if (dayNightPhase.dayIntensity > 0)
		{
			glEnable(GL_LIGHT0);
		}
		else
		{
			glDisable(GL_LIGHT0);
		}

		if (dayNightPhase.nightIntensity > 0)
		{
			glEnable(GL_LIGHT1);
		}
		else
		{
			glDisable(GL_LIGHT1);
		}
	}
	else
	{
		vec4 ambient(0, 0, 0, 1);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, glm::value_ptr(ambient));
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
	}
}

void EnvironmentManager::SetFog()
{
	if (_Settings->drawfog)
	{
		float fogdist = _Settings->fogdistance;
		float fogstart = 0.5f;

		_Settings->culldistance = fogdist;

		vec4 fogcolor(skies->colorSet[FOG_COLOR], 1);
		glFogfv(GL_FOG_COLOR, glm::value_ptr(fogcolor)); // TODO: retreive fogstart and fogend from lights.lit somehow
		glFogf(GL_FOG_START, fogdist * fogstart);
		glFogf(GL_FOG_END, fogdist);
		glEnable(GL_FOG);
	}
	else
	{
		glDisable(GL_FOG);
		_Settings->culldistance = _Settings->mapdrawdistance;
	}
}


//

void EnvironmentManager::BeforeDraw()
{
	m_HasSky = false;

	dayNightPhase = dayNightCycle->getPhase(Time_GetTime());
	skies->initSky(_Camera->Position, Time_GetTime());
}


