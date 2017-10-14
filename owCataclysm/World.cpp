#include "stdafx.h"

// General
#include "world.h"


World::World()
{
	// SHADERS
	mainCamera = new Camera;
	mainCamera->setupViewParams(45.0f, Modules::config().aspectRatio, 2.0f, 15000.0f);
	_PipelineGlobal->SetCamera(mainCamera);

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


	glGenBuffers(1, &m_VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

	vector<vec3> vertices;
	vertices.push_back(vec3(-1.0f, -1.0f, 0.0f));
	vertices.push_back(vec3(1.0f, -1.0f, 0.0f));
	vertices.push_back(vec3(-1.0f, 1.0f, 0.0f));

	vertices.push_back(vec3(-1.0f, 1.0f, 0.0f));
	vertices.push_back(vec3(1.0f, -1.0f, 0.0f));
	vertices.push_back(vec3(1.0f, 1.0f, 0.0f));
	
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

World::~World()
{
	Modules::log().Info("Unloaded world [%s]", _Map->GetFolder().c_str());

	// temp code until I figure out water properly
	//if (water)
	//	video.textures.del(water);


	_EnvironmentManager->Destroy();

	Modules::log().Info("World [%s] unloaded", _Map->GetFolder().c_str());
}

void World::drawShader(GLint _color)
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
	m_gbuffer->ClearFinalBuffer();

	// Geometry pass
	m_gbuffer->BindForGeomPass();
	m_gbuffer->Clear();
	//_Render->r->setFillMode(R_FillMode::RS_FILL_WIREFRAME);
	//_Render->r->commitStates();
	RenderGeom();

	// Postprocess pass
	m_gbuffer->BindForLightPass();
	//_Render->r->setFillMode(R_FillMode::RS_FILL_SOLID);
	//_Render->r->commitStates();
	RenderPostprocess();

	//
	// SECONDS PASS
	//
	/*
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
	m_gbuffer->Clear();
	RenderGeom();
	_PipelineGlobal->RenderCamera(_CameraFrustum);


	finalTexture2 = _Render->r->getTexture(_Render->r->getRenderBufferTex(m_gbuffer->rb2, 2)).glObj;

	m_gbuffer->BindForLightPass();*/
}

//***************************************

void World::RenderGeom()
{
	//------------------------------------------------------------------------------
	// Draw sky
	//------------------------------------------------------------------------------
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	_EnvironmentManager->skies->drawSky(_Camera->Position);


	//------------------------------------------------------------------------------
	// Draw sky from WMO
	//------------------------------------------------------------------------------
	//if (Modules::config().draw_map_mdx)
	//{
	_Map->RenderSky();
	//}

	//------------------------------------------------------------------------------
	// Draw sky from GLOBAL WMO
	//------------------------------------------------------------------------------
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
	glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	PERF_START(PERF_MAP_LOWRESOLUTION);
	if (/*Modules::config().drawfog &&*/ Modules::config().draw_map_chunk)
	{
		_TechniquesMgr->m_MapTileLowRes_GeometryPass->Bind();
		_Pipeline->Clear();
		_TechniquesMgr->m_MapTileLowRes_GeometryPass->SetPVW();
		_TechniquesMgr->m_MapTileLowRes_GeometryPass->SetShadowColor(_EnvironmentManager->GetSkyColor(FOG_COLOR));

		_Map->RenderLowResTiles();

		_TechniquesMgr->m_MapTileLowRes_GeometryPass->Unbind();
	}
	PERF_STOP(PERF_MAP_LOWRESOLUTION);

	//

	//------------------------------------------------------------------------------
	// Map chunks
	//------------------------------------------------------------------------------
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	PERF_START(PERF_MAP_CHUNK_GEOMETRY);
	if (Modules::config().draw_map_chunk)
	{
		Modules::config().uselowlod = Modules::config().drawfog;

		_TechniquesMgr->m_MapChunk_GeometryPass->Bind();
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
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (Modules::config().draw_map_chunk)
	{
		_Map->RenderWater();
	}
	
	//

	PERF_STOP(PERF_MAP);

	//

	//------------------------------------------------------------------------------
	// Global WMO
	//------------------------------------------------------------------------------
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);

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
	glDisable(GL_CULL_FACE);

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
	glDisable(GL_CULL_FACE);

	PERF_START(PERF_MAP_MODELS_MDXs);
	if (Modules::config().draw_map_mdx)
	{
		_Map->RenderModels();
	}
	PERF_STOP(PERF_MAP_MODELS_MDXs);

	glDisable(GL_BLEND);
}

void World::RenderPostprocess()
{
	//DSSimpleRenderPass();

	_EnvironmentManager->dayNightPhase.setupLighting();
	DSDirectionalLightPass(_EnvironmentManager->dayNightPhase.m_dirLightDay);
	DSDirectionalLightPass(_EnvironmentManager->dayNightPhase.m_dirLightNight);
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
	_TechniquesMgr->m_DSDirLightPassTech->Bind();
	_TechniquesMgr->m_DSDirLightPassTech->SetEyeWorldPos(_Camera->Position);
	_TechniquesMgr->m_DSDirLightPassTech->SetDirectionalLight(_light);
	_TechniquesMgr->m_DSDirLightPassTech->SetScreenSize(Modules::config().windowSizeX, Modules::config().windowSizeY);
	_TechniquesMgr->m_DSDirLightPassTech->BindToPostprocess();

	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisable(GL_BLEND);

	glEnable(GL_DEPTH_TEST);

	_TechniquesMgr->m_DSDirLightPassTech->Unbind();
}

void World::DSSimpleRenderPass()
{
	_TechniquesMgr->m_SimpleRender->Bind();
	_TechniquesMgr->m_SimpleRender->SetScreenSize(Modules::config().windowSizeX, Modules::config().windowSizeY);

	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisable(GL_BLEND);

	glEnable(GL_DEPTH_TEST);

	_TechniquesMgr->m_SimpleRender->Unbind();
}
