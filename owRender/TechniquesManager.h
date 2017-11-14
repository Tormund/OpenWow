#pragma once

// Log
#include "Techniques/Debug_GeometryPass.h"

#include "Techniques/Sky_GeometryPass.h"
#include "Techniques/MapTileLowRes_GeometryPass.h"
#include "Techniques/MapChunk_GeometryPass.h"
#include "Techniques/WMO_GeomertyPass.h"
#include "Techniques/Water_Pass.h"
#include "Techniques/Model_Shader.h"

// After
#include "Techniques/ds_dir_light_pass_tech.h"
#include "Techniques/POST_Fog.h"
#include "Techniques/SimpleRenderGBuffer.h"

// UI
#include "Techniques/UI_Color.h"
#include "Techniques/UI_Font.h"
#include "Techniques/UI_Texture.h"

class TechniquesMgr
{
	CLASS_INSTANCE(TechniquesMgr);

	TechniquesMgr();
	~TechniquesMgr();

	void Init();

	Debug_GeometryPass* m_Debug_GeometryPass;

	Sky_GeometryPass* m_Sky_GeometryPass;
	MapChunk_GeometryPass* m_MapChunk_GeometryPass;
	MapTileLowRes_GeometryPass* m_MapTileLowRes_GeometryPass;
	WMO_GeomertyPass* m_WMO_GeometryPass;
	Water_Pass* m_Water;
	Model_Shader* m_Model;

	// Lights

	DSDirLightPassTech* m_DSDirLightPassTech;
    POST_Fog* m_POST_Fog;
	SimpleRenderGBuffer* m_SimpleRender;

	// UI
	UI_Color* m_UI_Color;
	UI_Font* m_UI_Font;
	UI_Texture* m_UI_Texture;
};

#define _TechniquesMgr TechniquesMgr::instance()