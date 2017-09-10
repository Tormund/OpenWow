#include "stdafx.h"

// General
#include "world.h"

// Additional
#include "shaders.h"
#include "MapChunk.h"
#include "ModelsManager.h"
#include "WMO_Instance.h"

#include "WoWSettings.h"

#define COLOR_TEXTURE_UNIT              GL_TEXTURE0
#define COLOR_TEXTURE_UNIT_INDEX        0

#define SHADOW_TEXTURE_UNIT             GL_TEXTURE1
#define SHADOW_TEXTURE_UNIT_INDEX       1

#define NORMAL_TEXTURE_UNIT             GL_TEXTURE2
#define NORMAL_TEXTURE_UNIT_INDEX       2

#define RANDOM_TEXTURE_UNIT             GL_TEXTURE3
#define RANDOM_TEXTURE_UNIT_INDEX       3

#define DISPLACEMENT_TEXTURE_UNIT       GL_TEXTURE4
#define DISPLACEMENT_TEXTURE_UNIT_INDEX 4

#define SPECULAR_TEXTURE_UNIT           GL_TEXTURE5
#define SPECULAR_TEXTURE_UNIT_INDEX     5

World::World()
{
	// SHADERS
	auto camera = new Camera;
	_Pipeline->SetCamera(camera);
	_Pipeline->SetProjection(45.0f, 1024.0f / 768.0f, 0.1f, 10000.0f);


	m_gbuffer = new GBuffer();
	if (!m_gbuffer->Init(1024, 768))
	{
		Debug::Error("zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz");
		return;
	}

	// Geometry pass
	m_DSGeomPassTech = new DSGeomPassTech();
	if (!m_DSGeomPassTech->Init())
	{
		Debug::Error("Error initializing DSGeomPassTech\n");
		return;
	}
	m_DSGeomPassTech->Bind();
	m_DSGeomPassTech->SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
	m_DSGeomPassTech->SetSpecularTextureUnit(SPECULAR_TEXTURE_UNIT_INDEX);
	m_DSGeomPassTech->Unbind();

	mat4 nullMatrix;
	nullMatrix[0][0] = 1.0f; nullMatrix[0][1] = 0.0f; nullMatrix[0][2] = 0.0f; nullMatrix[0][3] = 0.0f;
	nullMatrix[1][0] = 0.0f; nullMatrix[1][1] = 1.0f; nullMatrix[1][2] = 0.0f; nullMatrix[1][3] = 0.0f;
	nullMatrix[2][0] = 0.0f; nullMatrix[2][1] = 0.0f; nullMatrix[2][2] = 1.0f; nullMatrix[2][3] = 0.0f;
	nullMatrix[3][0] = 0.0f; nullMatrix[3][1] = 0.0f; nullMatrix[3][2] = 0.0f; nullMatrix[3][3] = 1.0f;

	//

	// Point light
	m_DSPointLightPassTech = new DSPointLightPassTech();
	if (!m_DSPointLightPassTech->Init())
	{
		Debug::Error("Error initializing DSPointLightPassTech");
		return;
	}
	m_DSPointLightPassTech->Bind();
	m_DSPointLightPassTech->SetPositionTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
	m_DSPointLightPassTech->SetColorTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
	m_DSPointLightPassTech->SetNormalTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
	m_DSPointLightPassTech->SetSpecularTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_SPECULAR);
	m_DSPointLightPassTech->SetScreenSize(1024, 768);
	m_DSPointLightPassTech->SetMatSpecularPower(64);
	m_DSPointLightPassTech->SetWVP(nullMatrix);
	m_DSPointLightPassTech->Unbind();

	//

	// Directional light
	m_DSDirLightPassTech = new DSDirLightPassTech();
	if (!m_DSDirLightPassTech->Init())
	{
		Debug::Error("Error initializing DSDirLightPassTech");
		return;
	}
	m_DSDirLightPassTech->Bind();
	m_DSDirLightPassTech->SetPositionTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
	m_DSDirLightPassTech->SetColorTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
	m_DSDirLightPassTech->SetNormalTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
	m_DSDirLightPassTech->SetSpecularTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_SPECULAR);
	m_DSDirLightPassTech->SetScreenSize(1024, 768);
	m_DSDirLightPassTech->SetMatSpecularPower(64);
	m_DSDirLightPassTech->SetWVP(nullMatrix);
	m_DSDirLightPassTech->Unbind();

	//

	m_nullTech = new NullTechnique();
	if (!m_nullTech->Init())
	{
		Debug::Error("Error initializing NULL TECH\n");
		return;
	}

	colorPassTech = new SimpleColorTechnique();
	if (!colorPassTech->Init())
	{
		Debug::Error("Error initializing SimpleColorTech");
		return;
	}


	_WowSettings->CalculateSquareDistances();

	// Fog params
	l_const = 0.0f;
	l_linear = 0.7f;
	l_quadratic = 0.03f;

	// Other
	skies = 0;
	dayNightCycle = 0;
	mapstrip = 0;
	mapstrip2 = 0;

	// Common
	time = 1450;
	animtime = 0;
}

World::~World()
{
	Debug::Info("Unloaded world [%s]", m_map.GetPath().c_str());

	// temp code until I figure out water properly
	//if (water)
	//	video.textures.del(water);

	if (skies)
		delete skies;

	if (dayNightCycle)
		delete dayNightCycle;

	if (mapstrip)
		delete[] mapstrip;

	if (mapstrip2)
		delete[] mapstrip2;

	Debug::Info("World [%s] unloaded", m_map.GetPath().c_str());
}

//

GLuint gdetailtexcoords = 0, galphatexcoords = 0;

void initGlobalVBOs()
{
	if (gdetailtexcoords == 0 && galphatexcoords == 0)
	{

		GLuint detailtexcoords, alphatexcoords;

		vec2 temp[C_MapBufferSize], *vt;
		float tx, ty;

		// init texture coordinates for detail map:
		vt = temp;
		const float detail_half = 0.5f * detail_size / 8.0f;
		for (int j = 0; j < 17; j++)
		{
			for (int i = 0; i < ((j % 2) ? 8 : 9); i++)
			{
				tx = detail_size / 8.0f * i;
				ty = detail_size / 8.0f * j * 0.5f;
				if (j % 2)
				{
					// offset by half
					tx += detail_half;
				}
				*vt++ = vec2(tx, ty);
			}
		}

		glGenBuffersARB(1, &detailtexcoords);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, detailtexcoords);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, C_MapBufferSize * 2 * sizeof(float), temp, GL_STATIC_DRAW_ARB);

		// init texture coordinates for alpha map:
		vt = temp;
		const float alpha_half = 0.5f * 1.0f / 8.0f;
		for (int j = 0; j < 17; j++)
		{
			for (int i = 0; i < ((j % 2) ? 8 : 9); i++)
			{
				tx = 1.0f / 8.0f * i;
				ty = 1.0f / 8.0f * j * 0.5f;
				if (j % 2)
				{
					// offset by half
					tx += alpha_half;
				}
				//*vt++ = vec2(tx*0.95f, ty*0.95f);
				const int divs = 32;
				const float inv = 1.0f / divs;
				const float mul = (divs - 1.0f);
				*vt++ = vec2(tx*(mul*inv), ty*(mul*inv));
			}
		}

		glGenBuffersARB(1, &alphatexcoords);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, alphatexcoords);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, C_MapBufferSize * 2 * sizeof(float), temp, GL_STATIC_DRAW_ARB);

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);


		gdetailtexcoords = detailtexcoords;
		galphatexcoords = alphatexcoords;
	}

}


void World::initDisplay()
{
	// default strip indices
	short *defstrip = new short[stripsize];
	for (int i = 0; i < stripsize; i++)
		defstrip[i] = i; // note: this is ugly and should be handled in stripify
	mapstrip = new short[stripsize];
	stripify<short>(defstrip, mapstrip);
	delete[] defstrip;

	defstrip = new short[stripsize2];
	for (int i = 0; i < stripsize2; i++)
		defstrip[i] = i; // note: this is ugly and should be handled in stripify
	mapstrip2 = new short[stripsize2];
	stripify2<short>(defstrip, mapstrip2);
	delete[] defstrip;

	initGlobalVBOs();
	detailtexcoords = gdetailtexcoords;
	alphatexcoords = galphatexcoords;

	skies = new MapSkies(m_map.GetTemplate()->Get_ID());

	dayNightCycle = new DayNightCycle();
}

void World::outdoorLighting()
{
	vec4 black(0, 0, 0, 0);
	vec4 ambient(skies->colorSet[LIGHT_GLOBAL_AMBIENT], 1);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, glm::value_ptr(ambient));

	float di = dayNightPhase.dayIntensity, ni = dayNightPhase.nightIntensity;
	//di = 1;
	//ni = 0;

	//vec3 dd = outdoorLightStats.dayDir;
	// HACK: let's just keep the light source in place for now
	vec4 pos(-1, 1, -1, 0);
	vec4 col(skies->colorSet[LIGHT_GLOBAL_DIFFUSE] * di, 1);
	glLightfv(GL_LIGHT0, GL_AMBIENT, glm::value_ptr(black));
	glLightfv(GL_LIGHT0, GL_DIFFUSE, glm::value_ptr(col));
	glLightfv(GL_LIGHT0, GL_POSITION, glm::value_ptr(pos));

	const float spc = _WowSettings->useshaders ? 1.4f : 0; // specular light intensity...
	vec4 spcol(spc, spc, spc, 1);
	glLightfv(GL_LIGHT0, GL_SPECULAR, glm::value_ptr(spcol)); // ???


	/*
	vec3 dd = outdoorLightStats.nightDir;
	vec4 pos(-dd.x, -dd.z, dd.y, 0);
	vec4 col(skies->colorSet[LIGHT_GLOBAL_DIFFUSE] * ni, 1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, black);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, col);
	glLightfv(GL_LIGHT1, GL_POSITION, pos);
	*/
}

void World::outdoorLights(bool on)
{
	float di = dayNightPhase.dayIntensity, ni = dayNightPhase.nightIntensity;

	if (on)
	{
		vec4 ambient(skies->colorSet[LIGHT_GLOBAL_AMBIENT], 1);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, glm::value_ptr(ambient));
		if (di > 0)
		{
			glEnable(GL_LIGHT0);
		}
		else
		{
			glDisable(GL_LIGHT0);
		}
		if (ni > 0)
		{
			glEnable(GL_LIGHT1);
		}
		else
		{
			glDisable(GL_LIGHT1);
		}
	}
	else
	{
		vec4 ambient(0, 0, 0, 1);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, glm::value_ptr(ambient));
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
	}
}

void World::setupFog()
{
	if (_WowSettings->drawfog)
	{
		float fogdist = _WowSettings->fogdistance;
		float fogstart = 0.5f;

		_WowSettings->culldistance = fogdist;

		vec4 fogcolor(skies->colorSet[FOG_COLOR], 1);
		glFogfv(GL_FOG_COLOR, glm::value_ptr(fogcolor)); // TODO: retreive fogstart and fogend from lights.lit somehow
		glFogf(GL_FOG_START, fogdist * fogstart);
		glFogf(GL_FOG_END, fogdist);
		glEnable(GL_FOG);
	}
	else
	{
		glDisable(GL_FOG);
		_WowSettings->culldistance = _WowSettings->mapdrawdistance;
	}
}

void World::draw()
{
#ifdef WMO_INCL
	WMOInstance::reset();
#endif

#ifdef WMO_INCL
	_ModelsMgr->resetAnim();
#endif

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);


	// camera is set up
	frustum.retrieve();

	glDisable(GL_LIGHTING);
	glColor4f(1, 1, 1, 1);

	hadSky = false;
	m_map.RenderSky();

	if (m_map.MapHasGlobalWMO() && !hadSky)
	{
		m_map.SetOutOfBounds(false);
#ifdef WMO_INCL
		m_map.GetMapWMOs()->GetGlobalWMOInstance()->GetWMO()->drawSkybox();
#endif
	}

	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);

	int daytime = ((int)time) % 2880;
	dayNightPhase = dayNightCycle->getPhase(daytime);


	skies->initSky(_Camera->Position, daytime);

	if (!hadSky)
	{
		hadSky = skies->drawSky(vec3(_Camera->Position.x, _Camera->Position.y, _Camera->Position.z));
	}

	GLbitfield clearmask = GL_DEPTH_BUFFER_BIT;
	if (!hadSky)
	{
		clearmask |= GL_COLOR_BUFFER_BIT;
	}
	glClear(clearmask);

	glDisable(GL_TEXTURE_2D);


	outdoorLighting();
	outdoorLights(true);

	glFogi(GL_FOG_MODE, GL_LINEAR);
	setupFog();

	// Draw verylowres heightmap
	if (/*_WowSettings->drawfog && */_WowSettings->drawterrain)
	{
		glEnable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glColor3fv(glm::value_ptr(this->skies->colorSet[FOG_COLOR]));

		//glDisable(GL_FOG);
		m_map.RenderLowResTiles();
		//glEnable(GL_FOG);
	}

	// Draw height map
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	// Depth
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL); // less z-fighting artifacts this way, I think

	// Lighting
	glEnable(GL_LIGHTING);

	// Material
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glColor4f(1, 1, 1, 1);

	// if we're using shaders let's give it some specular
	if (supportShaders && _WowSettings->useshaders)
	{
		vec4 spec_color(1, 1, 1, 1);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(spec_color));
		glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 20);

		glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClientActiveTextureARB(GL_TEXTURE0_ARB);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, detailtexcoords);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);

	glClientActiveTextureARB(GL_TEXTURE1_ARB);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, alphatexcoords);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);

	glClientActiveTextureARB(GL_TEXTURE0_ARB);

	// height map w/ a zillion texture passes
	if (_WowSettings->drawterrain)
	{
		_WowSettings->uselowlod = _WowSettings->drawfog;
		m_map.RenderTiles();
	}

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_2D);

	glColor4f(1, 1, 1, 1);

	glEnable(GL_BLEND);

	if (supportShaders && _WowSettings->useshaders)
	{
		vec4 spec_color(0, 0, 0, 1);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(spec_color));
		glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 0);
	}

	// unbind hardware buffers
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);


	glEnable(GL_CULL_FACE);

	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);

	// TEMP: for fucking around with lighting
	for (int i = 0; i < 8; i++)
	{
		GLuint light = GL_LIGHT0 + i;
		glLightf(light, GL_CONSTANT_ATTENUATION, l_const);
		glLightf(light, GL_LINEAR_ATTENUATION, l_linear);
		glLightf(light, GL_QUADRATIC_ATTENUATION, l_quadratic);
	}

	if(m_map.MapHasGlobalWMO()) {
		m_map.SetOutOfBounds(false);
#ifdef WMO_INCL
		m_map.GetMapWMOs()->GetGlobalWMOInstance()->draw();
#endif
	}

	if (supportShaders && _WowSettings->useshaders)
	{
		vec4 spec_color(1, 1, 1, 1);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(spec_color));
		glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 10);

		glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
	}

	if (_WowSettings->drawwmo)
	{
		m_map.RenderObjects();
	}
	

	if (supportShaders && _WowSettings->useshaders)
	{
		vec4 spec_color(0, 0, 0, 1);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(spec_color));
		glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 0);
	}

	outdoorLights(true);
	setupFog();

	glColor4f(1, 1, 1, 1);

	if (_WowSettings->drawmodels)
	{
		m_map.RenderModels();
	}

	glDisable(GL_CULL_FACE);


	// ------ Water -------//

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (_WowSettings->drawterrain)
	{
		m_map.RenderWater();
	}

	glColor4f(1, 1, 1, 1);
	glDisable(GL_COLOR_MATERIAL);
}

void World::tick(float dt)
{
	_WowSettings->CalculateSquareDistances();

	m_map.Tick();

#ifdef MDX_INCL
	while (dt > 0.1f)
	{
		_ModelsMgr->updateEmitters(0.1f);
		dt -= 0.1f;
	}

	_ModelsMgr->updateEmitters(dt);
#endif
}
