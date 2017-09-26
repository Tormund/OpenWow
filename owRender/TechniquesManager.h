#pragma once

#include "Techniques\Billboard_GeometryPass.h"

#include "Techniques/Sky_GeometryPass.h"
#include "Techniques/MapTileLowRes_GeometryPass.h"
#include "Techniques/MapChunk_GeometryPass.h"
#include "Techniques/MDX_GeometryPass.h"
#include "Techniques/WMO_GeomertyPass.h"
#include "Techniques/WMO_MH2O_GeometryPass.h"

// After
#include "Techniques/ds_dir_light_pass_tech.h"
#include "Techniques/SimpleRenderGBuffer.h"

class TechniquesMgr
{
	CLASS_INSTANCE(TechniquesMgr);

	TechniquesMgr();
	~TechniquesMgr();

	void Init();

	Billboard_GeometryPass* m_Billboard;

	Sky_GeometryPass* m_Sky_GeometryPass;
	MapChunk_GeometryPass* m_MapChunk_GeometryPass;
	MapTileLowRes_GeometryPass* m_MapTileLowRes_GeometryPass;
	MDX_GeometryPass* m_MDX_GeometryPass;
	WMO_GeomertyPass* m_WMO_GeometryPass;
	WMO_MH2O_GeometryPass* m_WMO_MH2O_GeometryPass;

	// Lights

	DSDirLightPassTech* m_DSDirLightPassTech;

	SimpleRenderGBuffer* m_SimpleRender;
};

#define _TechniquesMgr TechniquesMgr::instance()