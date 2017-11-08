#include "stdafx.h"

// General
#include "world.h"

World::World()
{
	rb2 = _Render->r->createRenderBuffer(Modules::config().windowSizeX, Modules::config().windowSizeY, R_TextureFormats::RGBA16F, true, 4, 0);

	// Test camera
	testCamera = new Camera;
	testCamera->setupViewParams(45.0f, Modules::config().aspectRatio, 2.0f, 15000.0f);

	//----------------------------------------------------------------//

	Modules::config().CalculateSquareDistances();

	// Fog params
	l_const = 0.0f;
	l_linear = 0.7f;
	l_quadratic = 0.03f;
}

void World::Render()
{
	WMOInstance::reset();

	_ModelsMgr->resetAnim();

	_PipelineGlobal->SetCamera(_Render->mainCamera);
	_PipelineGlobal->SetCameraFrustum(_Render->mainCamera);

	_Perfomance->FrameBegin();
	_EnvironmentManager->BeforeDraw();
    _Render->mainCamera->onPostUpdate();
		
	// Main frame
	

	// Geometry pass
	_Render->r->setRenderBuffer(_Render->rb);
	_Render->r->clear();
	RenderGeom();

	// Postprocess pass
	_Render->r->setRenderBuffer(0);
	for (uint32 i = 0; i < 4; i++)
	{
		_Render->r->setTexture(i, _Render->r->getRenderBufferTex(_Render->rb, i), 0, R_TextureUsage::Texture);
	}
	_Render->r->clear(CLR_COLOR_RT0 | CLR_DEPTH);
	RenderPostprocess();

	//
	// SECONDS PASS
	//
	/*
	WMOInstance::reset();
	
	// Conf test camera
	testCamera->Position = mainCamera->Position + vec3(0, 1, 0) * 1000.0f;
	testCamera->Roll = mainCamera->Roll;
	testCamera->Pitch = -90.0f;
	testCamera->Update();

	// Test frame
	_PipelineGlobal->SetCamera(testCamera);

	// Geometry pass
	_Render->r->setRenderBuffer(rb2);
	_Render->r->clear();
	RenderGeom();
	//_PipelineGlobal->RenderCamera(_CameraFrustum);
	*/
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
		_Map->SetOutOfBounds(false);
		_Map->GetGlobalWMOInstance()->GetWMO()->drawSkybox();
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

        _Render->r->setCullMode(R_CullMode::RS_CULL_FRONT);

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
		_Map->SetOutOfBounds(false);
		_Map->GetGlobalWMOInstance()->Render();
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

	while (dt > 0.1f)
	{
		_ModelsMgr->updateEmitters(0.1f);
		dt -= 0.1f;
	}

	_ModelsMgr->updateEmitters(dt);
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
