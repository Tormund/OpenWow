#pragma once

// EngineLog
#include "Techniques\Debug_GeometryPass.h"

#include "Techniques/Sky_GeometryPass.h"
#include "Techniques/MapTileLowRes_GeometryPass.h"
#include "Techniques/MapChunk_GeometryPass.h"
#include "Techniques/MDX_GeometryPass.h"
#include "Techniques/WMO_GeomertyPass.h"
#include "Techniques/Water_Pass.h"

// After
#include "Techniques/ds_dir_light_pass_tech.h"
#include "Techniques/SimpleRenderGBuffer.h"

// UI
#include "Techniques/UI_Font.h"

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
	MDX_GeometryPass* m_MDX_GeometryPass;
	WMO_GeomertyPass* m_WMO_GeometryPass;
	Water_Pass* m_Water;

	// Lights

	DSDirLightPassTech* m_DSDirLightPassTech;

	SimpleRenderGBuffer* m_SimpleRender;

	// UI
	UI_Font* m_UI_Font;

};

#define _TechniquesMgr TechniquesMgr::instance()