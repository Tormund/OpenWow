#include "stdafx.h"

// General
#include "GameState_Menu.h"

GLint currentColor;

bool GameState_Menu::Init()
{
	_Input->AddInputListener(this);

	_ModulesMgr->LoadModule(_ModelsMgr, true);
	_ModulesMgr->LoadModule(_WMOsMgr, true);


	//_ModulesMgr->GetModule<UIWindow>()->Enable();

	enableFreeCamera = false;
	cameraSprint = false;

	minimapActive = false;

	window = new UIWindow();
	window->Init(VECTOR_ZERO, vec2(_Settings->windowSizeX, _Settings->windowSizeY), nullptr);
	_UIMgr->Attach(window);

	auto camera = new Camera;
	_Pipeline->SetCamera(camera);
	_Pipeline->SetProjection(45.0f, _Settings->aspectRatio, 0.1f, 10000.0f);

	const unsigned mapsXStart = 10;
	const unsigned mapsYStart = 10;

	unsigned mapsX = mapsXStart;
	unsigned mapsY = mapsYStart;
	const unsigned mapsXdelta = 135;
	const unsigned mapsYdelta = 25;

	int y = 0;

	for (auto i = gMapDB.Records()->begin(); i != gMapDB.Records()->end(); ++i)
	{
		auto record = (i->second);

		// Check
		if (mapsY + mapsYdelta > 768)
		{
			mapsX += mapsXdelta;
			mapsY = mapsYStart;
		}

		auto image = new Image(_TexturesMgr->Add("Interface\\Buttons\\UI-DialogBox-Button-Up.blp"), VECTOR_ZERO, vec2(128, 22));

		// Add btn
		auto btn = new UIButton();
		btn->Init(vec2(mapsX, mapsY), image);
		btn->Attach(window);
		btn->ShowText();
		btn->SetText(record->Get_Directory_cstr());
		SETBUTTONACTION_ARG(btn, GameState_Menu, this, OnBtn, gMapDBRecord*, record);

		mapsY += mapsYdelta;
	}

	cmd = CMD_NONE2;

	_TimeManager->globalTime = 0;
	_TimeManager->animtime = 0;
	

	backgroundModel = 0;
	//randBackground();


	currentColor = GL_COLOR_ATTACHMENT6;

	return true;
}

void GameState_Menu::Destroy()
{
	//window->Delete();

	m_inited = false;

	_Input->DeleteInputListener(this);
}

void GameState_Menu::InputPhase(double t, double dt)
{
	float delta = PI / 60.0f;
	float speed = 4.5f;

	if (cameraSlow)
	{
		speed *= 0.2f;
	}

	if (cameraSprint)
	{
		speed *= 3.0f;
	}

	if (_Input->IsKeyPressed(GLFW_KEY_W))
	{
		_Camera->ProcessKeyboard(FORWARD, speed);
	}

	if (_Input->IsKeyPressed(GLFW_KEY_S))
	{
		_Camera->ProcessKeyboard(BACKWARD, speed);
	}

	if (_Input->IsKeyPressed(GLFW_KEY_A))
	{
		_Camera->ProcessKeyboard(LEFT, speed);
	}

	if (_Input->IsKeyPressed(GLFW_KEY_D))
	{
		_Camera->ProcessKeyboard(RIGHT, speed);
	}
}

void GameState_Menu::UpdatePhase(double t, double dt)
{
	_TimeManager->animtime += dt * 1000.0f;
	_TimeManager->globalTime = (int)_TimeManager->animtime;

	_World->tick(dt);

	if (backgroundModel)
	{
		backgroundModel->updateEmitters(dt);
	}
}

void GameState_Menu::Render(double t, double dt)
{
	glDisable(GL_FOG);

	if (cmd == CMD_IN_WORLD2 && !minimapActive)
	{
		
		_World->drawShader(currentColor);
	}

	if (backgroundModel != nullptr)
	{
		vec4 la(0.1f, 0.1f, 0.1f, 1);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, glm::value_ptr(la));

		//***glEnable(GL_COLOR_MATERIAL);
		//***glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

		/****glColor4f(1, 1, 1, 1);
		for (int i = 0; i < 8; i++)
		{
			GLuint light = GL_LIGHT0 + i;
			glLightf(light, GL_CONSTANT_ATTENUATION, 0);
			glLightf(light, GL_LINEAR_ATTENUATION, 0.7f);
			glLightf(light, GL_QUADRATIC_ATTENUATION, 0.03f);
			glDisable(light);
		}****/

		/****glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);***/

		glEnable(GL_TEXTURE_2D);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		//****glEnable(GL_LIGHTING);

		backgroundModel->m_Cameras[0].setup(_TimeManager->globalTime);
		backgroundModel->draw();
	}
}

void GameState_Menu::RenderUI(double t, double dt)
{
	//if (_World->loading)
	//{
	//	_Render->RenderText(vec2(_Render->GetWindowSize().x / 2, 200), _World->GetMap()->IsOutOfBounds() ? "Out of bounds" : "Loading...");
	//}

	if (minimapActive || cmd == CMD_SELECT2)
	{
		int basex = 200;
		int basey = 0;

		if (_Map->GetMinimap() != 0)
		{
			const int len = 768;
			glColor4f(1, 1, 1, 1);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, _Map->GetMinimap());
			glBegin(GL_QUADS);
			{
				glTexCoord2f(0, 0);
				glVertex2i(basex, basey);
				glTexCoord2f(1, 0);
				glVertex2i(basex + len, basey);
				glTexCoord2f(1, 1);
				glVertex2i(basex + len, basey + len);
				glTexCoord2f(0, 1);
				glVertex2i(basex, basey + len);
			}
			glEnd();
			glDisable(GL_TEXTURE_2D);

			// Player position
			glBegin(GL_LINES);
			float fx, fz;
			fx = basex + _Camera->Position.x / C_TileSize * 12.0f;
			fz = basey + _Camera->Position.z / C_TileSize * 12.0f;
			glVertex2f(fx, fz);
			glColor4f(1, 1, 1, 0);
			glVertex2f(fx + 10.0f * cosf(glm::radians(_Camera->Yaw)), fz + 10.0f * sinf(glm::radians(_Camera->Yaw)));
			glEnd();
		}
	}

	if (cmd == CMD_LOAD_WORLD2)
	{
		_Render->RenderText(vec2(_Settings->windowSizeX / 2, 20 / 2), "Loading...");
		cmd = CMD_DO_LOAD_WORLD2;
	}
	else if (cmd == CMD_SELECT2)
	{
		if (_Map->GetTilesCount() == 0)
		{
			_Render->RenderText(vec2(400, 360), "Click to enter");
		}
		else
		{
			_Render->RenderText(vec2(400, 0), "Select your starting point");
		}
	}
	else if (cmd == CMD_IN_WORLD2)
	{ // HUD
// Skyname
//char* sn = _World->skies->getSkyName();
//if(sn)
//	_Render->RenderText(vec2(200, 0), string(sn));

// Area and region
		string areaName = "Unknown";
		string regionName = "Unknown";
		try
		{
			auto rec = gMapDB.getByID(_Map->getAreaID());
			areaName = rec->Get_Name_cstr();
			//regionName = AreaDB::getAreaName(rec.getUInt(AreaDB::Region));
		}
		catch (DBCNotFound)
		{
			areaName = "Not found!";
		}

		_Render->RenderText(vec2(5, 20), "Area: [" + areaName + "] [" + std::to_string(_Map->getAreaID()) + "]");
		_Render->RenderText(vec2(5, 40), "Region: " + regionName + "]");
		_Render->RenderText(vec2(5, 60), "CURRX: " + to_string(_Map->GetCurrentX()) + ", CURRZ " + to_string(_Map->GetCurrentZ()));

		_Render->RenderText(vec2(5, 100), "MAP_CHUNK: " + _Perfomance->Get(PERF_MAP_CHUNK));
		_Render->RenderText(vec2(5, 120), "MAP_LOWRES_TILES: " + _Perfomance->Get(PERF_MAP_LOWRES_TILES));
		_Render->RenderText(vec2(5, 140), "MAP_GLOBAL_WMO: " + _Perfomance->Get(PERF_GLOBAL_WMO));
		_Render->RenderText(vec2(5, 160), "MAP_MDXs: " + _Perfomance->Get(PERF_MAP_MDXs));
		_Render->RenderText(vec2(5, 180), "MAP_LIQUID: " + _Perfomance->Get(PERF_MAP_LIQUID));
		_Render->RenderText(vec2(5, 200), "MAP_MH20: " + _Perfomance->Get(PERF_MAP_MH20));

		_Render->RenderText(vec2(5, 240), "WMOs: " + _Perfomance->Get(PERF_WMOs));
		_Render->RenderText(vec2(5, 260), "WMOs_DOODADS: " + _Perfomance->Get(PERF_WMOs_DOODADS));
		_Render->RenderText(vec2(5, 280), "WMOs_LIQUIDS: " + _Perfomance->Get(PERF_WMOs_LIQUIDS));
		

		_Render->RenderText(vec2(5, 340), "SUMMA: " + _Perfomance->Sum());
		_Render->RenderText(vec2(5, 360), "FRUST: " + _Perfomance->Get(PERF_FRUSTRUM));
		_Render->RenderText(vec2(5, 380), "TEST: " + _Perfomance->Get(PERF_TEST));

		_Render->RenderText(vec2(5, _Settings->windowSizeY - 66), "REAL CamPos: [" + to_string(_Camera->Position.x) + "], [" + to_string(_Camera->Position.y) + "], [" + to_string(_Camera->Position.z) + "]");
		_Render->RenderText(vec2(5, _Settings->windowSizeY - 44), "CamPos: [" + to_string(-(_Camera->Position.x - C_ZeroPoint)) + "], [" + to_string(-(_Camera->Position.z - C_ZeroPoint)) + "], [" + to_string(_Camera->Position.y) + "]");
		_Render->RenderText(vec2(5, _Settings->windowSizeY - 22), "CamRot: [" + to_string(_Camera->Direction.x) + "], [" + to_string(_Camera->Direction.y) + "], [" + to_string(_Camera->Direction.z) + "]");

		// Time
		_Render->RenderText(vec2(_Settings->windowSizeX - 150, 0), "TIME [" + to_string(_EnvironmentManager->m_GameTime.GetHour()) + "." + to_string(_EnvironmentManager->m_GameTime.GetMinute()) + "]");
	
	}
}

//

bool GameState_Menu::LoadWorld(cvec3 _pos)
{
	_Map->enterTile(_pos.x / C_TileSize, _pos.z / C_TileSize);
	_World->initDisplay();


	_Camera->Position = _pos;
	_Camera->Update();

	if (backgroundModel != nullptr)
	{
		delete backgroundModel;
	}

	cmd = CMD_IN_WORLD2;

	return true;
}

//

#pragma region Input functional

MOUSE_MOVED_(GameState_Menu)
{
	if (cmd == CMD_IN_WORLD2 && enableFreeCamera)
	{
		vec2 mouseDelta = (_mousePos - lastMousePos) / _Settings->GetWindowSize();

		_Camera->ProcessMouseMovement(mouseDelta.x, -mouseDelta.y);

		_GLFW->SetMousePosition(lastMousePos);
	}
}

MOUSE_PRESSED(GameState_Menu)
{
	// Select point
	if (cmd == CMD_SELECT2 && _mousePos.x >= 200 && _mousePos.x < 200 + 12 * 64 && _mousePos.y < 12 * 64)
	{
		int selectedPointX = _mousePos.x - 200;
		int selectedPointZ = _mousePos.y;

		vec3 pointInWorld;

		if (_Map->MapHasTerrain())
		{
			pointInWorld = vec3(selectedPointX / 12.0f, 0.1f, selectedPointZ / 12.0f) * C_TileSize;
		}
		else if (_Map->MapHasGlobalWMO())
		{
#ifdef WMO_INCL
			pointInWorld = _Map->GetGlobalWMOPlacementInfo()->position;
#endif
		}

		delete backgroundModel;
		backgroundModel = nullptr;

		LoadWorld(pointInWorld);

		return true;
	}

	if (cmd == CMD_IN_WORLD2 && _button == GLFW_MOUSE_BUTTON_LEFT)
	{
		enableFreeCamera = true;
		lastMousePos = _mousePos;
		_GLFW->HideCursor();
		return true;
	}

	return false;
}

MOUSE_RELEASE(GameState_Menu)
{
	if (cmd == CMD_IN_WORLD2 && _button == GLFW_MOUSE_BUTTON_LEFT)
	{
		enableFreeCamera = false;
		lastMousePos = VECTOR_ZERO;
		_GLFW->ShowCursor();
		return true;
	}

	return false;
}

MOUSE_WHEEL(GameState_Menu)
{
	return false;
}

KEYBD_PRESSED(GameState_Menu)
{
	if (_key == GLFW_KEY_ESCAPE)
	{
		if (cmd == CMD_SELECT2)
		{
			cmd = CMD_NONE2;
			_UIMgr->Attach(window);
		}
		else if (cmd == CMD_IN_WORLD2)
		{
			cmd = CMD_SELECT2;
			_UIMgr->Attach(window);
		}
		else
		{
			Debug::Exit(0);
		}
	}

	if (_key == GLFW_KEY_X)
	{
		cameraSprint = true;
		return true;
	}

	if (_key == GLFW_KEY_Z)
	{
		cameraSlow = true;
		return true;
	}

	if (_key == GLFW_KEY_L)
	{
		_Settings->lighting = !_Settings->lighting;
		return true;
	}

	if (_key == GLFW_KEY_KP_1)
	{
		_Settings->draw_map_chunk = !_Settings->draw_map_chunk;
		return true;
	}
	if (_key == GLFW_KEY_KP_2)
	{
		_Settings->draw_map_wmo = !_Settings->draw_map_wmo;
		return true;
	}
	if (_key == GLFW_KEY_KP_3)
	{
		_Settings->draw_map_wmo_doodads = !_Settings->draw_map_wmo_doodads;
		return true;
	}

	if (_key == GLFW_KEY_KP_4)
	{
		_Settings->draw_map_mdx = !_Settings->draw_map_mdx;
		return true;
	}



	if (_key == GLFW_KEY_KP_7)
	{
		_Settings->disable_pipeline = !_Settings->disable_pipeline;
		return true;
	}

	if (_key == GLFW_KEY_KP_8)
	{
		_Settings->useOldMDXShader = !_Settings->useOldMDXShader;
		return true;
	}



	if (_key == GLFW_KEY_C)
	{
		_Settings->enableMCCV = !_Settings->enableMCCV;
		return true;
	}

	if (_key == GLFW_KEY_V)
	{
		_Settings->enableMCLV = !_Settings->enableMCLV;
		return true;
	}

	if (_key == GLFW_KEY_H)
	{
		_Settings->drawhighres = !_Settings->drawhighres;
		return true;
	}

	if (_key == GLFW_KEY_F)
	{
		_Settings->drawfog = !_Settings->drawfog;
		return true;
	}

	// minimap
	if (_key == GLFW_KEY_M)
	{
		minimapActive = !minimapActive;
		return true;
	}

	if (_key == GLFW_KEY_1)
	{
		currentColor = GL_COLOR_ATTACHMENT0;
		return true;
	}

	if (_key == GLFW_KEY_2)
	{
		currentColor = GL_COLOR_ATTACHMENT1;
		return true;
	}

	if (_key == GLFW_KEY_3)
	{
		currentColor = GL_COLOR_ATTACHMENT2;
		return true;
	}

	if (_key == GLFW_KEY_4)
	{
		currentColor = GL_COLOR_ATTACHMENT3;
		return true;
	}

	if (_key == GLFW_KEY_5)
	{
		currentColor = GL_COLOR_ATTACHMENT4;
		return true;
	}

	if (_key == GLFW_KEY_6)
	{
		currentColor = GL_COLOR_ATTACHMENT5;
		return true;
	}

	if (_key == GLFW_KEY_7)
	{
		currentColor = GL_COLOR_ATTACHMENT6;
		return true;
	}

	return false;
}

KEYBD_RELEASE(GameState_Menu)
{
	if (_key == GLFW_KEY_X)
	{
		cameraSprint = false;
		return true;
	}

	if (_key == GLFW_KEY_Z)
	{
		cameraSlow = false;
		return true;
	}

	return false;
}

#pragma endregion

void GameState_Menu::randBackground()
{
	if (backgroundModel != nullptr)
		delete backgroundModel;

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	char* ui[] = {"MainMenu", "NightElf", "Human", "Dwarf", "Orc", "Tauren", "Scourge"};

	char* randui = ui[Random::GenerateMax(7)];
	char path[256];
	//sprintf_s(path, "Interface\\Glues\\Models\\UI_%s\\UI_%s.m2", randui, randui);

	backgroundModel = new MDX("Interface\\Glues\\Models\\UI_Scourge\\UI_Scourge.m2");
	backgroundModel->Init();
	backgroundModel->m_IsBillboard = true;
}