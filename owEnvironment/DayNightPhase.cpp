#include "stdafx.h"

// General
#include "DayNightPhase.h"

void DayNightPhase::init(File& f)
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

	time = (int)h * 60 * 2 + (int)m * 2;

	// HACK: make day & night intensity exclusive; set day intensity to 1.0
	if (dayIntensity > 0)
	{
		dayIntensity = 1.0f;
		nightIntensity = 0.0f;
	}
}

void DayNightPhase::interpolate(DayNightPhase *a, DayNightPhase *b, float r)
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

void DayNightPhase::setupLighting()
{
	GLfloat ambient[4];
	GLfloat day[4];
	GLfloat direction[4];

	ambient[3] = 1.0f;
	day[3] = 1.0f;
	direction[3] = 0.0f;

	ambient[0] = ambient[1] = ambient[2] = 0.0f;

	// Setup day lighting
	if (dayIntensity > 0)
	{
		day[0] = dayColor.x * dayIntensity;
		day[1] = dayColor.y * dayIntensity;
		day[2] = dayColor.z * dayIntensity;

		direction[0] = dayDir.x;
		direction[1] = dayDir.z;
		direction[2] = -dayDir.y;

		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, day);
		glLightfv(GL_LIGHT0, GL_POSITION, direction);
		glEnable(GL_LIGHT0);
	}
	else
	{
		glDisable(GL_LIGHT0);
	}

	// Setup night lighting
	if (nightIntensity > 0)
	{
		day[0] = nightColor.x * nightIntensity;
		day[1] = nightColor.y * nightIntensity;
		day[2] = nightColor.z * nightIntensity;
		direction[0] = nightDir.x;
		direction[1] = nightDir.z;
		direction[2] = -nightDir.y;

		glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, day);
		glLightfv(GL_LIGHT1, GL_POSITION, direction);
		glEnable(GL_LIGHT1);
	}
	else
	{
		glDisable(GL_LIGHT1);
	}

	// Setup ambient lighting
	{
		ambient[0] = ambientColor.x * ambientIntensity;
		ambient[1] = ambientColor.y * ambientIntensity;
		ambient[2] = ambientColor.z * ambientIntensity;

		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
		glDisable(GL_LIGHT2);
	}
}
