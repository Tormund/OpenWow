#include "stdafx.h"

// General
#include "Settings.h"

Settings::Settings()
{
	// Distances
	highresdistance = 384.0f * 12.0f;
	mapdrawdistance = 998.0f * 12.0f;
	modeldrawdistance = 384.0f * 12.0f;
	doodaddrawdistance = 64.0f * 12.0f;

	fogdistance = 51200.0f;
	culldistance = fogdistance;

	// Draw enable flag
	draw_map_chunk = true;
	draw_map_wmo = true;
	draw_map_wmo_doodads = false;
	draw_map_mdx = true;

	disable_pipeline = false;

	useOldMDXShader = false;

	lighting = true;
	
	drawhighres = true;
	drawfog = false;
	drawColors = true;

	useshaders = true;


	enableMCCV = true;
	enableMCLV = true;

	SetWindowSize(1024, 768);
}
