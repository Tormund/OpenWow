#include "stdafx.h"

// General
#include "world.h"

World::World()
{
	// Main game camera
	mainCamera = new Camera;
	mainCamera->setupViewParams(45.0f, Modules::config().aspectRatio, 2.0f, 15000.0f);
	_PipelineGlobal->SetCamera(mainCamera);

	// Test camera
	testCamera = new Camera;
	testCamera->setupViewParams(45.0f, Modules::config().aspectRatio, 2.0f, 15000.0f);

	_EnvironmentManager->Init();

	//----------------------------------------------------------------//

	m_gbuffer = new GBuffer();
	m_gbuffer->Init();

	//----------------------------------------------------------------//

	Modules::config().CalculateSquareDistances();

	// Fog params
	l_const = 0.0f;
	l_linear = 0.7f;
	l_quadratic = 0.03f;

	_TechniquesMgr->Init();
}

void World::Render()
{
#ifdef WMO_INCL
	WMOInstance::reset();
#endif

#ifdef MDX_INCL
	_ModelsMgr->resetAnim();
#endif

	_PipelineGlobal->SetCamera(mainCamera);
	_PipelineGlobal->SetCameraFrustum(mainCamera);

	_Perfomance->FrameBegin();
	_EnvironmentManager->BeforeDraw();
	mainCamera->onPostUpdate();
		
	// Main frame
	

	// Geometry pass
	m_gbuffer->BindForGeomPass();
	_Render->r->clear();
	RenderGeom();

	// Postprocess pass
	m_gbuffer->BindForLightPass();
	m_gbuffer->ClearFinalBuffer();
	RenderPostprocess();

	//
	// SECONDS PASS
	//
	
#ifdef WMO_INCL
	WMOInstance::reset();
#endif
	
	// Conf test camera
	testCamera->Position = mainCamera->Position + vec3(0, 1, 0) * 1000.0f;
	testCamera->Roll = mainCamera->Roll;
	testCamera->Pitch = -90.0f;
	testCamera->Update();

	// Test frame
	_PipelineGlobal->SetCamera(testCamera);

	// Geometry pass
	m_gbuffer->BindForGeomPass2();
	_Render->r->clear();
	RenderGeom();
	//_PipelineGlobal->RenderCamera(_CameraFrustum);

	_Render->r->setRenderBuffer(0);
}

//***************************************

void World::RenderGeom()
{
	//------------------------------------------------------------------------------
	// Draw sky
	//------------------------------------------------------------------------------
	_Render->r->setDepthTest(false);
	_EnvironmentManager->skies->drawSky(_Camera->Position);

	
	//------------------------------------------------------------------------------
	// Draw sky from WMO
	//------------------------------------------------------------------------------
	_Render->r->setDepthTest(false);
	//if (Modules::config().draw_map_mdx)
	//{
	_Map->RenderSky();
	//}

	//------------------------------------------------------------------------------
	// Draw sky from GLOBAL WMO
	//------------------------------------------------------------------------------
	_Render->r->setDepthTest(true);
	if (_Map->MapHasGlobalWMO() && !_EnvironmentManager->m_HasSky)
	{
#ifdef WMO_INCL
#ifdef MDX_INCL
		_Map->SetOutOfBounds(false);
		_Map->GetGlobalWMOInstance()->GetWMO()->drawSkybox();
#endif
#endif
	}

	//
	PERF_START(PERF_MAP);
	//

	//------------------------------------------------------------------------------
	// Map low-resolution tiles
	//------------------------------------------------------------------------------
	_Render->r->setDepthTest(false);

	PERF_START(PERF_MAP_LOWRESOLUTION);
	if (Modules::config().drawfog && Modules::config().draw_map_chunk)
	{
		_TechniquesMgr->m_MapTileLowRes_GeometryPass->BindS();
		_Pipeline->Clear();
		_TechniquesMgr->m_MapTileLowRes_GeometryPass->SetPVW();
		_TechniquesMgr->m_MapTileLowRes_GeometryPass->SetShadowColor(_EnvironmentManager->GetSkyColor(FOG_COLOR));

		_Map->RenderLowResTiles();

		_TechniquesMgr->m_MapTileLowRes_GeometryPass->Unbind();
	}
	PERF_STOP(PERF_MAP_LOWRESOLUTION);

	_Render->r->setDepthTest(true);

	//

	//------------------------------------------------------------------------------
	// Map chunks
	//------------------------------------------------------------------------------
	PERF_START(PERF_MAP_CHUNK_GEOMETRY);
	if (Modules::config().draw_map_chunk)
	{
		Modules::config().uselowlod = Modules::config().drawfog;

		_TechniquesMgr->m_MapChunk_GeometryPass->BindS();
		_Pipeline->Clear();
		_TechniquesMgr->m_MapChunk_GeometryPass->SetPVW();

		_Map->RenderTiles();

		_TechniquesMgr->m_MapChunk_GeometryPass->Unbind();
	}
	PERF_STOP(PERF_MAP_CHUNK_GEOMETRY);

	//

	//------------------------------------------------------------------------------
	// Map water
	//------------------------------------------------------------------------------
	_Render->r->setCullMode(R_CullMode::RS_CULL_NONE);
	_Render->r->setBlendMode(true, R_BlendFunc::BS_BLEND_SRC_ALPHA, R_BlendFunc::BS_BLEND_INV_SRC_ALPHA);

	if (Modules::config().draw_map_chunk)
	{
		_Map->RenderWater();
	}

	_Render->r->setBlendMode(false);
	_Render->r->setCullMode(R_CullMode::RS_CULL_BACK);
	//

	PERF_STOP(PERF_MAP);

	//

	//------------------------------------------------------------------------------
	// Global WMO
	//------------------------------------------------------------------------------
	PERF_START(PERF_MAP_MODELS_WMO_GLOBAL);
	if (_Map->MapHasGlobalWMO())
	{
#ifdef WMO_INCL
		_Map->SetOutOfBounds(false);
		_Map->GetGlobalWMOInstance()->Render();
#endif
	}
	PERF_STOP(PERF_MAP_MODELS_WMO_GLOBAL);

	//

	//------------------------------------------------------------------------------
	// WMOs
	//------------------------------------------------------------------------------
	PERF_START(PERF_MAP_MODELS_WMOs);
	if (Modules::config().draw_map_wmo)
	{
		_Map->RenderObjects();
	}
	PERF_STOP(PERF_MAP_MODELS_WMOs);
	
	//

	//------------------------------------------------------------------------------
	// Map MDXs
	//------------------------------------------------------------------------------
	PERF_START(PERF_MAP_MODELS_MDXs);
	if (Modules::config().draw_map_mdx)
	{
		_Map->RenderModels();
	}
	PERF_STOP(PERF_MAP_MODELS_MDXs);
}

void World::RenderPostprocess()
{
	DSSimpleRenderPass();

	/*_EnvironmentManager->dayNightPhase.setupLighting();
	DSDirectionalLightPass(_EnvironmentManager->dayNightPhase.m_dirLightDay);
	DSDirectionalLightPass(_EnvironmentManager->dayNightPhase.m_dirLightNight);*/
}

void World::tick(float dt)
{
	Modules::config().CalculateSquareDistances();

	_Map->Tick();

#ifdef MDX_INCL
	while (dt > 0.1f)
	{
		_ModelsMgr->updateEmitters(0.1f);
		dt -= 0.1f;
	}

	_ModelsMgr->updateEmitters(dt);
#endif
}

void World::DSDirectionalLightPass(DirectionalLight& _light)
{
	_TechniquesMgr->m_DSDirLightPassTech->BindS();
	_TechniquesMgr->m_DSDirLightPassTech->SetEyeWorldPos(_Camera->Position);
	_TechniquesMgr->m_DSDirLightPassTech->SetDirectionalLight(_light);
	_TechniquesMgr->m_DSDirLightPassTech->SetScreenSize(Modules::config().windowSizeX, Modules::config().windowSizeY);
	_TechniquesMgr->m_DSDirLightPassTech->BindToPostprocess();

	_Render->r->setDepthTest(false);
	_Render->r->setBlendMode(true, R_BlendFunc::BS_BLEND_ONE, R_BlendFunc::BS_BLEND_ONE);

	_Render->RenderQuad();

	_Render->r->setBlendMode(false);
	_Render->r->setDepthTest(true);

	_TechniquesMgr->m_DSDirLightPassTech->Unbind();
}

void World::DSSimpleRenderPass()
{
	_TechniquesMgr->m_SimpleRender->BindS();
	_TechniquesMgr->m_SimpleRender->SetScreenSize(Modules::config().windowSizeX, Modules::config().windowSizeY);

	_Render->r->setDepthTest(false);
	_Render->r->setBlendMode(true, R_BlendFunc::BS_BLEND_ONE, R_BlendFunc::BS_BLEND_ONE);

	_Render->RenderQuad();

	_Render->r->setBlendMode(false);
	_Render->r->setDepthTest(true);

	_TechniquesMgr->m_SimpleRender->Unbind();
}
