#include "stdafx.h"

// General
#include "Settings.h"

	// Render settings
float Settings::windowSizeX;
float Settings::windowSizeY;
float Settings::aspectRatio;
float Settings::aspectFactor;

// Distances
float Settings::mapdrawdistance = 998.0f * 12.0f;
float Settings::modeldrawdistance = 384.0f * 12.0f;
float Settings::doodaddrawdistance = 64.0f * 12.0f;
float Settings::highresdistance = 384.0f * 12.0f;
float Settings::culldistance;

float Settings::mapdrawdistance2;
float Settings::modeldrawdistance2;
float Settings::doodaddrawdistance2;
float Settings::highresdistance2;
float Settings::culldistance2;

float Settings::fogdistance = 51200.0f;

// Drawing
bool Settings::draw_map_chunk = true;
bool Settings::draw_map_wmo = true;
bool Settings::draw_map_wmo_doodads = true;
bool Settings::draw_map_mdx = true;


bool Settings::loading;
bool Settings::drawhighres;
bool Settings::drawfog;

// Quality
bool Settings::uselowlod = true;
bool Settings::enableMCCV = true;
bool Settings::enableMCLV = true;

// Perfomance
bool Settings::disable_pipeline = false;

//

void Settings::Init()
{
	SetWindowSize(1280, 1024);
}
