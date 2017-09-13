#include "stdafx.h"

// General
#include "WoWSettings.h"

WoWSettings::WoWSettings()
{
	// Distances
	highresdistance = 384.0f * 12.0f;
	mapdrawdistance = 998.0f * 12.0f;
	modeldrawdistance = 384.0f * 12.0f;
	doodaddrawdistance = 64.0f * 12.0f;

	fogdistance = 512.0f;
	culldistance = fogdistance;

	// Draw enable flag
	lighting = true;
	drawmodels = true;
	drawdoodads = true;
	drawterrain = true;
	drawwmo = true;
	drawhighres = true;
	drawfog = false;
	drawColors = true;

	useshaders = true;
}
