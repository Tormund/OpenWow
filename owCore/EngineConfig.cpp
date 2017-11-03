#include "stdafx.h"

// General
#include "EngineConfig.h"

//

EngineConfig::EngineConfig()
{
	SetWindowSize(1280, 1024);

	mapdrawdistance = 998.0f * 12.0f;
	modeldrawdistance = 384.0f * 12.0f;
	doodaddrawdistance = 64.0f * 12.0f;
	highresdistance = 384.0f * 12.0f;

	fogdistance = 51200.0f;

	// Drawing
	draw_map_chunk = true;
	draw_map_wmo = true;
	draw_map_wmo_doodads = true;
	draw_map_mdx = true;

	// Quality
	uselowlod = true;
	enableMCCV = true;
	enableMCLV = true;

	// Perfomance

}

EngineConfig::~EngineConfig()
{}