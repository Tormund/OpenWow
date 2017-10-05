#include "stdafx.h"

// General
#include "DayNightPhase.h"

DayNightPhase::DayNightPhase()
{}

DayNightPhase::DayNightPhase(File& f)
{
	float h, m;

	f.SeekRelative(4); // Always 0x46
	f.ReadBytes(&h, 4);
	f.SeekRelative(4);
	f.ReadBytes(&m, 4);

	f.SeekRelative(4);
	f.ReadBytes(&dayIntensity, 4);
	//
	f.SeekRelative(4);
	f.ReadBytes(&dayColor.x, 4);
	f.SeekRelative(4);
	f.ReadBytes(&dayColor.y, 4);
	f.SeekRelative(4);
	f.ReadBytes(&dayColor.z, 4);
	//
	f.SeekRelative(4);
	f.ReadBytes(&dayDir.x, 4);
	f.SeekRelative(4);
	f.ReadBytes(&dayDir.y, 4);
	f.SeekRelative(4);
	f.ReadBytes(&dayDir.z, 4);

	//

	f.SeekRelative(4);
	f.ReadBytes(&nightIntensity, 4);
	//
	f.SeekRelative(4);
	f.ReadBytes(&nightColor.x, 4);
	f.SeekRelative(4);
	f.ReadBytes(&nightColor.y, 4);
	f.SeekRelative(4);
	f.ReadBytes(&nightColor.z, 4);
	//
	f.SeekRelative(4);
	f.ReadBytes(&nightDir.x, 4);
	f.SeekRelative(4);
	f.ReadBytes(&nightDir.y, 4);
	f.SeekRelative(4);
	f.ReadBytes(&nightDir.z, 4);

	//

	f.SeekRelative(4);
	f.ReadBytes(&ambientIntensity, 4);
	//
	f.SeekRelative(4);
	f.ReadBytes(&ambientColor.x, 4);
	f.SeekRelative(4);
	f.ReadBytes(&ambientColor.y, 4);
	f.SeekRelative(4);
	f.ReadBytes(&ambientColor.z, 4);

	//

	f.SeekRelative(4);
	f.ReadBytes(&fogDepth, 4);
	f.SeekRelative(4);
	f.ReadBytes(&fogIntensity, 4);
	//
	f.SeekRelative(4);
	f.ReadBytes(&fogColor.x, 4);
	f.SeekRelative(4);
	f.ReadBytes(&fogColor.y, 4);
	f.SeekRelative(4);
	f.ReadBytes(&fogColor.z, 4);

	uint32 time = (uint32)h * 60 * 2 + (uint32)m * 2;

	m_dirLightDay.ambient = vec3();
	m_dirLightDay.diffuse = vec3();
	m_dirLightDay.Direction = vec3();

	m_dirLightNight.ambient = vec3();
	m_dirLightNight.diffuse = vec3();
	m_dirLightNight.Direction = vec3();
}

DayNightPhase::DayNightPhase(DayNightPhase* a, DayNightPhase* b, float r)
{
	float ir = 1.0f - r;

	// Day
	dayIntensity = a->dayIntensity * ir + b->dayIntensity * r;
	dayColor = a->dayColor * ir + b->dayColor * r;
	dayDir = a->dayDir * ir + b->dayDir * r;

	// Night
	nightIntensity = a->nightIntensity * ir + b->nightIntensity * r;
	nightColor = a->nightColor * ir + b->nightColor * r;
	nightDir = a->nightDir * ir + b->nightDir * r;

	// Ambient
	ambientIntensity = a->ambientIntensity * ir + b->ambientIntensity * r;
	ambientColor = a->ambientColor * ir + b->ambientColor * r;

	// Fog
	fogDepth = a->fogDepth * ir + b->fogDepth * r;
	fogIntensity = a->fogIntensity * ir + b->fogIntensity * r;
	fogColor = a->fogColor * ir + b->fogColor * r;
}

//

void DayNightPhase::setupLighting()
{
	// Setup day lighting
	if (dayIntensity > 0)
	{
		m_dirLightDay.ambient = ambientColor * ambientIntensity * dayIntensity;
		m_dirLightDay.diffuse = dayColor * dayIntensity;
		m_dirLightDay.Direction = From_XYZ_To_XZminusY_RET(dayDir);
	}

	// Setup night lighting
	if (nightIntensity > 0)
	{
		m_dirLightNight.ambient = ambientColor * ambientIntensity * nightIntensity;
		m_dirLightNight.diffuse = nightColor * nightIntensity;
		m_dirLightNight.Direction = From_XYZ_To_XZminusY_RET(nightDir);
	}
}
