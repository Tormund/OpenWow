#include "stdafx.h"

// General
#include "RenderConfig.h"

// Additional
#include "RenderEnums.h"

RenderConfig::RenderConfig()
{
	SetWindowSize(1280, 1024);

	mapdrawdistance = 998.0f * 12.0f;
	modeldrawdistance = 384.0f * 12.0f;
	doodaddrawdistance = 64.0f * 12.0f;
	highresdistance = 384.0f * 12.0f;

	fogdistance = 51200.0f;

    // Render
    OpenGL.EXT_texture_filter_anisotropic = false;
    OpenGL.EXT_texture_compression_s3tc = false;
    OpenGL.EXT_texture_sRGB = false;
    OpenGL.OES_EGL_image = false;
    OpenGL.majorVersion = 1;
    OpenGL.minorVersion = 0;

    Quality.Terrain_LowDetail = false;
    Quality.Terrain_MCCV = true; 
    Quality.Terrain_MCLV = true;
    Quality.Texture_Sampler = R_SamplerState::SS_FILTER_POINT;

	// Drawing
	draw_map_chunk = true;
	draw_map_wmo = true;
	draw_map_wmo_doodads = true;
	draw_map_mdx = true;

	// Quality

	// Perfomance

}

RenderConfig::~RenderConfig()
{}