#include "stdafx.h"

// General
#include "GameState_Menu.h"

// Additional
#include "GLFWBackend.h"
#include "Engine.h"

bool GameState_Menu::Init()
{
	Modules::input().AddInputListener(this);

	_ModelsMgr->Init();
	_WMOsMgr->Init();

	enableFreeCamera = false;
	cameraSprint = false;

	minimapActive = false;

	window = new UIWindow();
	window->Init(VECTOR_ZERO, vec2(Modules::config().windowSizeX, Modules::config().windowSizeY), nullptr);
	_UIMgr->Attach(window);

	_EnvironmentManager->globalTime = 0;
	_EnvironmentManager->animtime = 0;

	cmd = CMD_NONE2;
	backgroundModel = 0;
	//randBackground();

	/*_Map->Load_WDT(DBC_Map[1]);
	LoadWorld(vec3(17644, 68, 17823));
	return true;*/

	const unsigned mapsXStart = 10;
	const unsigned mapsYStart = 10;

	unsigned mapsY[4];
	for (uint32 i = 0; i < 4; i++)
		mapsY[i] = mapsYStart;



	const unsigned mapsYdelta = 17;

	const uint32 mapsXClassic = 0;
	const uint32 mapsXBurning = 200;
	const uint32 mapsXWrathOf = 400;
	const uint32 mapsXCataclm = 800;

	int y = 0;

	for (auto i = DBC_Map.Records()->begin(); i != DBC_Map.Records()->end(); ++i)
	{
		auto record = (i->second);

		if (!MPQFile::IsFileExists("World\\Maps\\" + string(record->Get_Directory()) + "\\" + string(record->Get_Directory()) + ".wdt"))
		{
			continue;
		}

		auto image = new Image(_TexturesMgr->Add("Interface\\Buttons\\UI-DialogBox-Button-Up.blp"), VECTOR_ZERO, vec2(128, 22));

		// Add btn
		auto btn = new UIButton();


		btn->Init(vec2(100 + 200 * record->Get_ExpansionID(), mapsY[record->Get_ExpansionID()]), image);


		btn->Attach(window);
		btn->ShowText();
		btn->SetText(record->Get_Name());
		SETBUTTONACTION_ARG(btn, GameState_Menu, this, OnBtn, DBC_MapRecord*, record);

		mapsY[record->Get_ExpansionID()] += mapsYdelta;
	}

	return true;
}

void GameState_Menu::Destroy()
{
	//window->Delete();

	m_inited = false;

	Modules::input().DeleteInputListener(this);
}

void GameState_Menu::InputPhase(double t, double dt)
{
	float speed = 4.5f;

	if (cameraSlow)
		speed *= 0.2f;

	if (cameraSprint)
		speed *= 3.0f;

	if (Modules::input().IsKeyPressed(GLFW_KEY_W))
		_World->mainCamera->ProcessKeyboard(FORWARD, speed);

	if (Modules::input().IsKeyPressed(GLFW_KEY_S))
		_World->mainCamera->ProcessKeyboard(BACKWARD, speed);

	if (Modules::input().IsKeyPressed(GLFW_KEY_A))
		_World->mainCamera->ProcessKeyboard(LEFT, speed);

	if (Modules::input().IsKeyPressed(GLFW_KEY_D))
		_World->mainCamera->ProcessKeyboard(RIGHT, speed);
}

void GameState_Menu::UpdatePhase(double t, double dt)
{
	_EnvironmentManager->animtime += (dt * 1000.0f);
	_EnvironmentManager->globalTime = (int)_EnvironmentManager->animtime;

	_World->tick(dt);

	if (backgroundModel)
	{
		backgroundModel->updateEmitters(dt);
	}
}

void GameState_Menu::Render(double t, double dt)
{
	if (cmd == CMD_IN_WORLD2 && !minimapActive)
	{
		_World->Render();
	}

	if (backgroundModel != nullptr)
	{
		backgroundModel->m_Cameras[0].setup(_EnvironmentManager->globalTime);
		_PipelineGlobal->SetCamera(backgroundModel->m_Cameras[0].GetCamera());
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

			_Render->RenderTexture(vec2(basex, basey), _Map->GetMinimap(), vec2(len, len));

			// Player position
			/*glBegin(GL_LINES);
			float fx, fz;
			fx = basex + _World->mainCamera->Position.x / C_TileSize * 12.0f;
			fz = basey + _World->mainCamera->Position.z / C_TileSize * 12.0f;
			glVertex2f(fx, fz);
			glColor4f(1, 1, 1, 0);
			glVertex2f(fx + 10.0f * cosf(degToRad(_World->mainCamera->Roll)), fz + 10.0f * sinf(degToRad(_World->mainCamera->Roll)));
			glEnd();*/
		}
	}

	if (cmd == CMD_LOAD_WORLD2)
	{
		_Render->RenderText(vec2(Modules::config().windowSizeX / 2, 20 / 2), "Loading...");
		cmd = CMD_DO_LOAD_WORLD2;
	}
	else if (cmd == CMD_SELECT2)
	{
		if (_Map->MapHasTiles())
		{
			_Render->RenderText(vec2(400, 0), "Select your starting point");
		}
		else
		{
			_Render->RenderText(vec2(400, 360), "Click to enter");
		}
	}
	else if (cmd == CMD_IN_WORLD2)
	{ // HUD
// Skyname
//char* sn = _World->skies->getSkyName();
//if(sn)
//	_Render->RenderText(vec2(200, 0), string(sn));

// Area and region

		// Area
		DBÑ_AreaTableRecord* areaRecord = nullptr;
		string areaName = "<unknown>";

		areaRecord = DBÑ_AreaTable[_Map->getAreaID()];
		if (areaRecord != nullptr)
		{
			areaName = areaRecord->Get_Name();
		}

		// Region
		DBÑ_AreaTableRecord* regionRecord = nullptr;
		string regionName = "<unknown>";

		if (areaRecord != nullptr)
		{
			regionRecord = areaRecord->Get_ParentAreaID();
			if (regionRecord != nullptr)
			{
				regionName = regionRecord->Get_Name();
			}
		}


		_Render->RenderText(vec2(5, 20), "Area: [" + areaName + "] [Area id = " + std::to_string(_Map->getAreaID()) + "]");
		_Render->RenderText(vec2(5, 40), "Region: [" + regionName + "]");
		_Render->RenderText(vec2(5, 60), "CURRX: " + to_string(_Map->GetCurrentX()) + ", CURRZ " + to_string(_Map->GetCurrentZ()));


		///

		_Perfomance->Draw(vec2(5, 100));

		_Render->RenderText(vec2(5, Modules::config().windowSizeY - 66), "REAL CamPos: [" + to_string(_World->mainCamera->Position.x) + "], [" + to_string(_World->mainCamera->Position.y) + "], [" + to_string(_World->mainCamera->Position.z) + "]");
		_Render->RenderText(vec2(5, Modules::config().windowSizeY - 44), "CamPos: [" + to_string(-(_World->mainCamera->Position.x - C_ZeroPoint)) + "], [" + to_string(-(_World->mainCamera->Position.z - C_ZeroPoint)) + "], [" + to_string(_World->mainCamera->Position.y) + "]");
		_Render->RenderText(vec2(5, Modules::config().windowSizeY - 22), "CamRot: [" + to_string(_World->mainCamera->Direction.x) + "], [" + to_string(_World->mainCamera->Direction.y) + "], [" + to_string(_World->mainCamera->Direction.z) + "]");

		// Time
		_Render->RenderText(vec2(Modules::config().windowSizeX - 150, 0), "TIME [" + to_string(_EnvironmentManager->m_GameTime.GetHour()) + "." + to_string(_EnvironmentManager->m_GameTime.GetMinute()) + "]");
		char buff[256];

		// Ambient

		sprintf(buff, "Amb[c=[%0.2f %0.2f %0.2f] i=[%f]]",
				_EnvironmentManager->dayNightPhase.ambientColor.x, _EnvironmentManager->dayNightPhase.ambientColor.y, _EnvironmentManager->dayNightPhase.ambientColor.z,
				_EnvironmentManager->dayNightPhase.ambientIntensity
		);
		_Render->RenderText(vec2(Modules::config().windowSizeX - 400, 20), buff);

		// Day

		sprintf(buff, "Day[c=[%0.2f %0.2f %0.2f] i=[%f] d=[%0.2f %0.2f %0.2f]]",
				_EnvironmentManager->dayNightPhase.dayColor.x, _EnvironmentManager->dayNightPhase.dayColor.y, _EnvironmentManager->dayNightPhase.dayColor.z,
				_EnvironmentManager->dayNightPhase.dayIntensity,
				_EnvironmentManager->dayNightPhase.dayDir.x, _EnvironmentManager->dayNightPhase.dayDir.y, _EnvironmentManager->dayNightPhase.dayDir.z
		);
		_Render->RenderText(vec2(Modules::config().windowSizeX - 400, 40), buff);

		// Night

		sprintf(buff, "Nig[c=[%0.2f %0.2f %0.2f] i=[%f] d=[%0.2f %0.2f %0.2f]]\0",
				_EnvironmentManager->dayNightPhase.nightColor.x, _EnvironmentManager->dayNightPhase.nightColor.y, _EnvironmentManager->dayNightPhase.nightColor.z,
				_EnvironmentManager->dayNightPhase.nightIntensity,
				_EnvironmentManager->dayNightPhase.nightDir.x, _EnvironmentManager->dayNightPhase.nightDir.y, _EnvironmentManager->dayNightPhase.nightDir.z
		);
		_Render->RenderText(vec2(Modules::config().windowSizeX - 400, 60), buff);

		//_Render->RenderRectangle(vec2(Modules::config().windowSizeX * 2.0 / 3.0, Modules::config().windowSizeY * 2.0 / 3.0), vec2(Modules::config().windowSizeX / 3, Modules::config().windowSizeY / 3));
		//_Render->RenderTexture(vec2(Modules::config().windowSizeX * 2.0 / 3.0, Modules::config().windowSizeY * 2.0 / 3.0), _Map->GetMinimap()/*_Render->r->getRenderBufferTex(_World->m_gbuffer->rb2, 2)*/, vec2(Modules::config().windowSizeX / 3, Modules::config().windowSizeY / 3));
	}
}

//

bool GameState_Menu::LoadWorld(cvec3 _pos)
{
	_Map->enterTile(_pos.x / C_TileSize, _pos.z / C_TileSize);

	_World->mainCamera->Position = _pos;
	_World->mainCamera->Update();

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
		vec2 mouseDelta = (_mousePos - lastMousePos) / Modules::config().GetWindowSize();

		_World->mainCamera->ProcessMouseMovement(mouseDelta.x, -mouseDelta.y);

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

		if (_Map->MapHasTiles())
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
			_Engine->Destroy();
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

	if (_key == GLFW_KEY_KP_1)
	{
		Modules::config().draw_map_chunk = !Modules::config().draw_map_chunk;
		return true;
	}
	if (_key == GLFW_KEY_KP_2)
	{
		Modules::config().draw_map_wmo = !Modules::config().draw_map_wmo;
		return true;
	}
	if (_key == GLFW_KEY_KP_3)
	{
		Modules::config().draw_map_wmo_doodads = !Modules::config().draw_map_wmo_doodads;
		return true;
	}

	if (_key == GLFW_KEY_KP_4)
	{
		Modules::config().draw_map_mdx = !Modules::config().draw_map_mdx;
		return true;
	}



	if (_key == GLFW_KEY_KP_7)
	{
		Modules::config().disable_pipeline = !Modules::config().disable_pipeline;
		return true;
	}


	if (_key == GLFW_KEY_C)
	{
		Modules::config().enableMCCV = !Modules::config().enableMCCV;
		return true;
	}

	if (_key == GLFW_KEY_V)
	{
		Modules::config().enableMCLV = !Modules::config().enableMCLV;
		return true;
	}

	if (_key == GLFW_KEY_H)
	{
		Modules::config().drawhighres = !Modules::config().drawhighres;
		return true;
	}

	if (_key == GLFW_KEY_F)
	{
		Modules::config().drawfog = !Modules::config().drawfog;
		return true;
	}

	// minimap
	if (_key == GLFW_KEY_M)
	{
		minimapActive = !minimapActive;
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

	char* ui[] = {"MainMenu", "NightElf", "Human", "Dwarf", "Orc", "Tauren", "Scourge"};

	char* randui = ui[Random::GenerateMax(7)];
	char path[256];
	//sprintf_s(path, "Interface\\Glues\\Models\\UI_%s\\UI_%s.m2", randui, randui);

	backgroundModel = new MDX("Interface\\Glues\\Models\\UI_Scourge\\UI_Scourge.m2");
	backgroundModel->Init();
	backgroundModel->m_IsBillboard = true;
}