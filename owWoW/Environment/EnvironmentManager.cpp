#include "../stdafx.h"

// General
#include "EnvironmentManager.h"


bool EnvironmentManager::Init()
{
	m_GameTime.Set(12, 45);

	skies = 0;
	dayNightCycle = new DayNightCycle();

	// Colors
    m_OutdoorAmbientColor = vec4();
	m_OutdoorDayDiffuseColor = vec4();
	m_OutdoorNightDiffuseColor = vec4();
	m_OutdoorSpecularColor = vec4();
	
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

void EnvironmentManager::InitSkies(uint32 mapid)
{
	skies = new MapSkies(mapid);
}





void EnvironmentManager::outdoorLighting()
{
	m_OutdoorAmbientColor = vec4(skies->colorSet[LIGHT_GLOBAL_AMBIENT], 1.0f); // BLACK?

	m_OutdoorDayDiffuseColor = vec4(skies->colorSet[LIGHT_GLOBAL_DIFFUSE] * dayNightPhase.dayIntensity, 1.0f);
	m_OutdoorNightDiffuseColor = vec4(skies->colorSet[LIGHT_GLOBAL_DIFFUSE] * dayNightPhase.nightIntensity, 1.0f);

	m_OutdoorSpecularColor = vec4(1.4f, 1.4f, 1.4f, 1.0f);
}



void EnvironmentManager::SetAmbientLights(bool on)
{
	if (on)
	{
		vec4 ambient(skies->colorSet[LIGHT_GLOBAL_AMBIENT], 1);
		//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

		if (dayNightPhase.dayIntensity > 0)
		{
			//glEnable(GL_LIGHT0);
		}
		else
		{
			//glDisable(GL_LIGHT0);
		}

		if (dayNightPhase.nightIntensity > 0)
		{
			//glEnable(GL_LIGHT1);
		}
		else
		{
			//glDisable(GL_LIGHT1);
		}
	}
	else
	{
		vec4 ambient(0, 0, 0, 1);
		//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
		//glDisable(GL_LIGHT0);
		//glDisable(GL_LIGHT1);
	}
}

void EnvironmentManager::SetFog()
{
	if (_Config.drawfog)
	{
		float fogdist = _Config.fogdistance;
		float fogstart = 0.5f;

		_Config.culldistance = fogdist;

		vec4 fogcolor(skies->colorSet[FOG_COLOR], 1);
		//glFogfv(GL_FOG_COLOR, fogcolor); // TODO: retreive fogstart and fogend from lights.lit somehow
		//glFogf(GL_FOG_START, fogdist * fogstart);
		//glFogf(GL_FOG_END, fogdist);
		//glEnable(GL_FOG);
	}
	else
	{
		//glDisable(GL_FOG);
		_Config.culldistance = _Config.mapdrawdistance;
	}
}

//

void EnvironmentManager::BeforeDraw()
{
	m_GameTime.Tick();

	m_HasSky = false;

	dayNightPhase = dayNightCycle->getPhase(m_GameTime.GetTime());
	skies->initSky(_Camera->Position, m_GameTime.GetTime());
}


