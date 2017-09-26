#include "stdafx.h"

// General
#include "world.h"

World::World()
{
	// SHADERS
	auto camera = new Camera;
	_Pipeline->SetCamera(camera);
	_Pipeline->SetProjection(45.0f, _Settings->aspectRatio, 1.0f, 10000.0f);


	_EnvironmentManager->Init();

	//----------------------------------------------------------------//

	m_gbuffer = new GBuffer();
	m_gbuffer->Init(_Settings->windowSizeX, _Settings->windowSizeY);

	//----------------------------------------------------------------//

	_Settings->CalculateSquareDistances();

	// Fog params
	l_const = 0.0f;
	l_linear = 0.7f;
	l_quadratic = 0.03f;

	_TechniquesMgr->Init();
}

World::~World()
{
	Debug::Info("Unloaded world [%s]", _Map->GetPath().c_str());

	// temp code until I figure out water properly
	//if (water)
	//	video.textures.del(water);


	_EnvironmentManager->Destroy();

	Debug::Info("World [%s] unloaded", _Map->GetPath().c_str());
}

void World::initDisplay()
{
	_EnvironmentManager->InitSkies(_Map->GetTemplate()->Get_ID());
}

void World::draw(GLint _color)
{
#ifdef WMO_INCL
	WMOInstance::reset();
#endif

#ifdef WMO_INCL
	_ModelsMgr->resetAnim();
#endif

	// camera is set up
	_Render->frustum.retrieve();

	//****glDisable(GL_LIGHTING);
	glColor4f(1, 1, 1, 1);

	//****_Map->RenderSky();

	if (_Map->MapHasGlobalWMO() && !_EnvironmentManager->m_HasSky)
	{
#ifdef WMO_INCL
#ifdef MDX_INCL
		_Map->SetOutOfBounds(false);
		_Map->GetGlobalWMOInstance()->GetWMO()->drawSkybox();
#endif
#endif
	}

	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);

	_EnvironmentManager->BeforeDraw();

	if (!_EnvironmentManager->m_HasSky)
	{
		_EnvironmentManager->m_HasSky = _EnvironmentManager->skies->drawSky(_Camera->Position);
	}

	GLbitfield clearmask = GL_DEPTH_BUFFER_BIT;
	if (!_EnvironmentManager->m_HasSky)
	{
		clearmask |= GL_COLOR_BUFFER_BIT;
	}
	glClear(clearmask);

	glDisable(GL_TEXTURE_2D);


	_EnvironmentManager->outdoorLighting();
	_EnvironmentManager->SetAmbientLights(true);

	glFogi(GL_FOG_MODE, GL_LINEAR);
	_EnvironmentManager->SetFog();


	// Draw verylowres heightmap
	if (_Settings->drawfog && _Settings->draw_map_chunk)
	{
		glEnable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glColor3fv(glm::value_ptr(_EnvironmentManager->GetSkyColor(FOG_COLOR)));

		_Map->RenderLowResTiles();
	}

	// Draw height map
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	//_MapsMgr->ActivateTextureBuffers();

	glClientActiveTextureARB(GL_TEXTURE0_ARB);

	// Depth
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL); // less z-fighting artifacts this way, I think

	// Lighting
	//*****glEnable(GL_LIGHTING);

	// Material
	//*****glEnable(GL_COLOR_MATERIAL);
	//*****glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glColor4f(1, 1, 1, 1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// height map w/ a zillion texture passes
	if (_Settings->draw_map_chunk)
	{
		_Settings->uselowlod = _Settings->drawfog;
			
		//****_Map->RenderTiles(m_MapChunk_GeometryPass);
	}

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_2D);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	/*
	glEnable(GL_CULL_FACE);

	glDisable(GL_ALPHA_TEST);

	if (_Map->MapHasGlobalWMO())
	{
#ifdef WMO_INCL
		_Map->SetOutOfBounds(false);
		_Map->GetMapWMOs()->GetGlobalWMOInstance()->draw();
#endif
	}

	if (_Settings->draw_map_wmo)
	{
		_Map->RenderObjects();
	}

	_EnvironmentManager->SetAmbientLights(true);
	_EnvironmentManager->SetFog();

	glColor4f(1, 1, 1, 1);

	if (_Settings->draw_map_mdx)
	{
		_Map->RenderModels();
	}

	glDisable(GL_CULL_FACE);


	// ------ Water -------//

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (_Settings->draw_map_chunk)
	{
		//_Map->RenderWater();
	}

	glColor4f(1, 1, 1, 1);
	//*****glDisable(GL_COLOR_MATERIAL);


	*/
}

void World::drawShader(GLint _color)
{
#ifdef WMO_INCL
	WMOInstance::reset();
#endif

#ifdef WMO_INCL
	_ModelsMgr->resetAnim();
#endif

	_Perfomance->FrameBegin();
	_Render->frustum.retrieve();
	_EnvironmentManager->BeforeDraw();

	//============================== SHADER BEGIN
	m_gbuffer->StartFrame();
	DSGeometryPassBegin();

	m_gbuffer->Clear();

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	//------------------------------------------------------------------------------
	// Draw sky
	//------------------------------------------------------------------------------
	_EnvironmentManager->skies->drawSky(_Camera->Position);


	//------------------------------------------------------------------------------
	// Draw sky from WMO
	//------------------------------------------------------------------------------
	//if (_Settings->draw_map_mdx)
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

	/*glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	DSGeometryPassEnd();
	DSSimpleRenderPass222();
	m_gbuffer->BindForFinalPass(_color);
	m_gbuffer->Clear();
	//m_gbuffer->StartFrame();
	DSGeometryPassBegin();*/


	//------------------------------------------------------------------------------
	// Map low-resolution tiles
	//------------------------------------------------------------------------------
	glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	_Perfomance->Start(PERF_MAP_LOWRES_TILES);
	if (/*_Settings->drawfog &&*/ _Settings->draw_map_chunk)
	{
		_TechniquesMgr->m_MapTileLowRes_GeometryPass->Bind();
		_Pipeline->Clear();
		_TechniquesMgr->m_MapTileLowRes_GeometryPass->SetPVW();
		_TechniquesMgr->m_MapTileLowRes_GeometryPass->SetWorldMatrix(*_Pipeline->GetWorld());
		_TechniquesMgr->m_MapTileLowRes_GeometryPass->SetShadowColor(_EnvironmentManager->GetSkyColor(FOG_COLOR));

		_Map->RenderLowResTiles();

		_TechniquesMgr->m_MapTileLowRes_GeometryPass->Unbind();
	}
	_Perfomance->Stop(PERF_MAP_LOWRES_TILES);


	//------------------------------------------------------------------------------
	// Map chunks
	//------------------------------------------------------------------------------
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	_Perfomance->Start(PERF_MAP_CHUNK);
	if (_Settings->draw_map_chunk)
	{
		_Settings->uselowlod = _Settings->drawfog;

		_TechniquesMgr->m_MapChunk_GeometryPass->Bind();
		_Pipeline->Clear();
		_TechniquesMgr->m_MapChunk_GeometryPass->SetPVW();
		_TechniquesMgr->m_MapChunk_GeometryPass->SetWorldMatrix(*_Pipeline->GetWorld());

		_Map->RenderTiles();
		
		_TechniquesMgr->m_MapChunk_GeometryPass->Unbind();
	}
	_Perfomance->Stop(PERF_MAP_CHUNK);


	glDisable(GL_BLEND);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//------------------------------------------------------------------------------
	// Global WMO
	//------------------------------------------------------------------------------
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	_Perfomance->Start(PERF_GLOBAL_WMO);
	if (_Map->MapHasGlobalWMO())
	{
#ifdef WMO_INCL
		_Map->SetOutOfBounds(false);
		_Map->GetGlobalWMOInstance()->draw();
		
#endif
	}
	_Perfomance->Stop(PERF_GLOBAL_WMO);


	//------------------------------------------------------------------------------
	// WMOs
	//------------------------------------------------------------------------------
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	_Perfomance->Start(PERF_WMOs);
	if (_Settings->draw_map_wmo)
	{
		_Map->RenderObjects();
	}
	_Perfomance->Stop(PERF_WMOs);

	//------------------------------------------------------------------------------
	// Map MDXs
	//------------------------------------------------------------------------------
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_Perfomance->Start(PERF_MAP_MDXs);
	if (_Settings->draw_map_mdx)
	{
		_Map->RenderModels();
	}
	_Perfomance->Stop(PERF_MAP_MDXs);

	//------------------------------------------------------------------------------
	// Map water
	//------------------------------------------------------------------------------
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//if (_Settings->draw_map_chunk)
	{
		_TechniquesMgr->m_WMO_MH2O_GeometryPass->Bind();
		_Pipeline->Clear();
		_TechniquesMgr->m_WMO_MH2O_GeometryPass->SetPVW();
		_TechniquesMgr->m_WMO_MH2O_GeometryPass->SetWorldMatrix(*_Pipeline->GetWorld());

		_Map->RenderWater();

		_TechniquesMgr->m_WMO_MH2O_GeometryPass->Unbind();
	}

	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	DSGeometryPassEnd();


	//---------------------------
	//---------------------------
	//---------------------------


	//DSSimpleRenderPass();


	// LIGHTS, FOG ...
	DirectionalLight dirLight;
	dirLight.ambient = _EnvironmentManager->GetSkyColor(LIGHT_GLOBAL_AMBIENT);
	dirLight.diffuse = vec3(0,0,0);
	dirLight.Direction.x = 0;
	dirLight.Direction.y = 0;
	dirLight.Direction.z = 0;
	DSDirectionalLightPass(dirLight);


	//DSDirectionalLightPass(_EnvironmentManager->dayNightPhase.m_dirLightNight);
	
	

	m_gbuffer->BindForFinalPass(_color);
}

void World::tick(float dt)
{
	_Settings->CalculateSquareDistances();

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




void World::DSGeometryPassBegin()
{
	m_gbuffer->BindForGeomPass();

	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
}

void World::DSGeometryPassEnd()
{
	glDepthMask(GL_FALSE);
}

void World::DSDirectionalLightPass(DirectionalLight& _light)
{
	m_gbuffer->BindForLightPass();

	_TechniquesMgr->m_DSDirLightPassTech->Bind();
	_TechniquesMgr->m_DSDirLightPassTech->SetEyeWorldPos(_Camera->Position);
	_TechniquesMgr->m_DSDirLightPassTech->SetDirectionalLight(_light);

	_TechniquesMgr->m_DSDirLightPassTech->BindToPostprocess();

	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	glBegin(GL_QUADS);
	{
		glVertex3f(1.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);
	}
	glEnd();

	glDisable(GL_BLEND);

	glEnable(GL_DEPTH_TEST);

	_TechniquesMgr->m_DSDirLightPassTech->Unbind();
}

void World::DSSimpleRenderPass()
{
	m_gbuffer->BindForLightPass();

	_TechniquesMgr->m_SimpleRender->Bind();

	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	glBegin(GL_QUADS);
	{
		glVertex3f(1.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);
	}
	glEnd();

	glDisable(GL_BLEND);

	glEnable(GL_DEPTH_TEST);

	_TechniquesMgr->m_SimpleRender->Unbind();
}
