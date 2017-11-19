#include "stdafx.h"

// General
#include "RenderConfig.h"

// Additional
#include "RenderEnums.h"

RenderConfig::RenderConfig()
{
	SetWindowSize(1280, 1024);

	Distances.mapdrawdistance = 998.0f * 12.0f;
    Distances.modeldrawdistance = 384.0f * 12.0f;
    Distances.doodaddrawdistance = 64.0f * 12.0f;
    Distances.highresdistance = 384.0f * 12.0f;
    Distances.fogdistance = 51200.0f;

    // Render
    OpenGL.EXT_texture_filter_anisotropic = false;
    OpenGL.EXT_texture_compression_s3tc = false;
    OpenGL.EXT_texture_sRGB = false;
    OpenGL.OES_EGL_image = false;
    OpenGL.majorVersion = 1;
    OpenGL.minorVersion = 0;

    // Quality
    Quality.Terrain_LowDetail = false;
    Quality.Terrain_MCCV = true; 
    Quality.Terrain_MCLV = true;
    Quality.Texture_Sampler = R_SamplerState::SS_ANISO16;

	// Drawing
	draw_map_chunk = true;
	draw_map_wmo = true;
	draw_map_wmo_doodads = true;
	draw_map_mdx = true;
    draw_water = true;

    timeEnable = false;
}

RenderConfig::~RenderConfig()
{}
