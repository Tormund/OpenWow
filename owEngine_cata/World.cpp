#include "stdafx.h"

// General
#include "world.h"

World::World()
{
	// SHADERS
	auto camera = new Camera;
	_Pipeline->SetCamera(camera);
	_Pipeline->SetProjection(45.0f, _Settings->aspectRatio, 0.1f, 10000.0f);


	_EnvironmentManager->Init();

	//----------------------------------------------------------------//

	m_gbuffer = new GBuffer();
	if (!m_gbuffer->Init(_Settings->windowSizeX, _Settings->windowSizeY))
	{
		Debug::Error("Error initializing GBuffer");
		return;
	}

	//----------------------------------------------------------------//

	m_DSGeomPassTech = new DSGeomPassTech();
	if (!m_DSGeomPassTech->Init())
	{
		Debug::Error("Error initializing DSGeomPassTech");
		return;
	}
	m_DSGeomPassTech->Bind();
	m_DSGeomPassTech->SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
	m_DSGeomPassTech->SetSpecularTextureUnit(SPECULAR_TEXTURE_UNIT_INDEX);
	m_DSGeomPassTech->Unbind();

	//----------------------------------------------------------------//

	m_SimpleRender = new SimpleRenderGBuffer();
	if (!m_SimpleRender->Init())
	{
		Debug::Error("Error initializing SimpleRenderGBuffer");
		return;
	}
	m_SimpleRender->Bind();
	m_SimpleRender->SetWVP(glm::mat4(1.0f));

	m_SimpleRender->SetPositionTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
	m_SimpleRender->SetColorTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
	m_SimpleRender->SetNormalTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
	m_SimpleRender->SetSpecularTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_SPECULAR);

	m_SimpleRender->SetScreenSize(_Settings->windowSizeX, _Settings->windowSizeY);
	m_SimpleRender->Unbind();



	pass = new MapTilePass();
	if (!pass->Init())
	{
		Debug::Error("Error initializing DSGeomPassTech");
		return;
	}
	pass->Bind();

	pass->SetColorTextureUnit0(0);
	pass->SetColorTextureUnit1(1);
	pass->SetColorTextureUnit2(2);
	pass->SetColorTextureUnit3(3);

	pass->SetBlendBuffer(4);

	pass->SetSpecularTextureUnit(7);
	pass->Unbind();

	//

	_Settings->CalculateSquareDistances();

	// Fog params
	l_const = 0.0f;
	l_linear = 0.7f;
	l_quadratic = 0.03f;

}

World::~World()
{
	Debug::Info("Unloaded world [%s]", _MapsManager->GetMap()->GetPath().c_str());

	// temp code until I figure out water properly
	//if (water)
	//	video.textures.del(water);


	_EnvironmentManager->Destroy();

	Debug::Info("World [%s] unloaded", _MapsManager->GetMap()->GetPath().c_str());
}

void World::initDisplay()
{
	_MapsManager->CreateTextureBuffers();

	_EnvironmentManager->InitSkies(_MapsManager->GetMap()->GetTemplate()->Get_ID());
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

	//****_MapsManager->GetMap()->RenderSky();

	if (_MapsManager->GetMap()->MapHasGlobalWMO() && !_EnvironmentManager->m_HasSky)
	{
#ifdef WMO_INCL
#ifdef MDX_INCL
		_MapsManager->GetMap()->SetOutOfBounds(false);
		_MapsManager->GetMap()->GetMapWMOs()->GetGlobalWMOInstance()->GetWMO()->drawSkybox();
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
	if (_Settings->drawfog && _Settings->drawterrain)
	{
		glEnable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glColor3fv(glm::value_ptr(_EnvironmentManager->GetSkyColor(FOG_COLOR)));

		_MapsManager->GetMap()->RenderLowResTiles();
	}

	//============================== SHADER BEGIN
	m_gbuffer->StartFrame();
	DSGeometryPassBegin();

	// Draw height map
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	_MapsManager->ActivateTextureBuffers();

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
	if (_Settings->drawterrain)
	{
		_Settings->uselowlod = _Settings->drawfog;
			
		_MapsManager->GetMap()->RenderTiles(pass);
	}

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_2D);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	/*
	glEnable(GL_CULL_FACE);

	glDisable(GL_ALPHA_TEST);

	if (_MapsManager->GetMap()->MapHasGlobalWMO())
	{
#ifdef WMO_INCL
		_MapsManager->GetMap()->SetOutOfBounds(false);
		_MapsManager->GetMap()->GetMapWMOs()->GetGlobalWMOInstance()->draw();
#endif
	}

	if (_Settings->drawwmo)
	{
		_MapsManager->GetMap()->RenderObjects();
	}

	_EnvironmentManager->SetAmbientLights(true);
	_EnvironmentManager->SetFog();

	glColor4f(1, 1, 1, 1);

	if (_Settings->drawmodels)
	{
		_MapsManager->GetMap()->RenderModels();
	}

	glDisable(GL_CULL_FACE);


	// ------ Water -------//

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (_Settings->drawterrain)
	{
		//_MapsManager->GetMap()->RenderWater();
	}

	glColor4f(1, 1, 1, 1);
	//*****glDisable(GL_COLOR_MATERIAL);


	*/

	//DSGeometryPassEnd();

	DSSimpleRenderPass();
	m_gbuffer->BindForFinalPass(_color);
}

void World::drawShader(GLint _color)
{
#ifdef WMO_INCL
	WMOInstance::reset();
#endif

#ifdef WMO_INCL
	_ModelsMgr->resetAnim();
#endif

	// camera is set up
	//_Render->frustum.retrieve();

	GLbitfield clearmask = GL_DEPTH_BUFFER_BIT;
	if (!_EnvironmentManager->m_HasSky)
	{
		clearmask |= GL_COLOR_BUFFER_BIT;
	}
	glClear(clearmask);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//============================== SHADER BEGIN
	m_gbuffer->StartFrame();
	DSGeometryPassBegin();

	// height map w/ a zillion texture passes
	if (_Settings->drawterrain)
	{
		_Settings->uselowlod = _Settings->drawfog;

		pass->Bind();

		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		_Pipeline->Clear();
		pass->SetWVP(*_Pipeline->GetPVM());
		pass->SetWorldMatrix(*_Pipeline->GetWorld());

		_MapsManager->GetMap()->RenderTiles(pass);

		pass->Unbind();
	}


	DSGeometryPassEnd();

	DSSimpleRenderPass();
	m_gbuffer->BindForFinalPass(_color);
}

void World::tick(float dt)
{
	_Settings->CalculateSquareDistances();

	_MapsManager->GetMap()->Tick();

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

	/*m_DSGeomPassTech->Bind();

	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_Pipeline->Clear();
	m_DSGeomPassTech->SetWVP(*_Pipeline->GetPVM());
	m_DSGeomPassTech->SetWorldMatrix(*_Pipeline->GetWorld());*/
}

void World::DSGeometryPassEnd()
{
	glDepthMask(GL_FALSE);
	//m_DSGeomPassTech->Unbind();
}

void World::DSSimpleRenderPass()
{
	m_gbuffer->BindForLightPass();

	m_SimpleRender->Bind();

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

	m_SimpleRender->Unbind();
}
