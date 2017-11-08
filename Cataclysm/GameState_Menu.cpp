#include "stdafx.h"

// General
#include "GameState_Menu.h"

// Additional
#include "GameState_InWorld.h"

bool GameState_Menu::Init()
{
    GameState::Init();

    //

	_ModelsMgr->Init();
	_WMOsMgr->Init();

	OpenDBs();

    m_MinimapTexture = new Texture();

    m_MinimapUI = new UIElement(100);
    m_MinimapUI->Init(vec2(200, 0), vec2(768, 768), m_MinimapTexture, COLOR_WHITE);
    m_MinimapUI->Hide();

    _EnvironmentManager->Init();
    _TechniquesMgr->Init();
	_EnvironmentManager->globalTime = 0;
	_EnvironmentManager->animtime = 0;

	cmd = CMD_NONE2;
	backgroundModel = 0;
	randBackground();

	//_Map->Load_WDT(DBC_Map[1]);
	//LoadWorld(vec3(17644, 68, 17823));
	//return true;

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


		btn->AttachTo(m_Window);
		btn->ShowText();
		btn->SetText(record->Get_Name());
		SETBUTTONACTION_ARG(btn, GameState_Menu, this, OnBtn, DBC_MapRecord*, record);

		mapsY[record->Get_ExpansionID()] += mapsYdelta;
	}

    //
    enableFreeCamera = false;
    cameraSprint = false;
    //

    return true;
}

void GameState_Menu::Destroy()
{
    GameState::Destroy();
}

//

bool GameState_Menu::Set()
{
    GameState::Set();

    cmd = CMD_NONE2;

    return true;
}

void GameState_Menu::Unset()
{
    GameState::Unset();
}

//

void GameState_Menu::Input(double t, double dt)
{
    float speed = 4.5f;

    if (cameraSlow)
        speed *= 0.2f;

    if (cameraSprint)
        speed *= 3.0f;

    if (Modules::input().IsKeyPressed(OW_KEY_W))
        _Render->mainCamera->ProcessKeyboard(FORWARD, speed);

    if (Modules::input().IsKeyPressed(OW_KEY_S))
        _Render->mainCamera->ProcessKeyboard(BACKWARD, speed);

    if (Modules::input().IsKeyPressed(OW_KEY_A))
        _Render->mainCamera->ProcessKeyboard(LEFT, speed);

    if (Modules::input().IsKeyPressed(OW_KEY_D))
        _Render->mainCamera->ProcessKeyboard(RIGHT, speed);
}

void GameState_Menu::Update(double t, double dt)
{
	_EnvironmentManager->animtime += (dt * 1000.0f);
	_EnvironmentManager->globalTime = (int)_EnvironmentManager->animtime;

	if (backgroundModel)
	{
		backgroundModel->updateEmitters(dt);
	}
}

void GameState_Menu::Render(double t, double dt)
{
	if (backgroundModel != nullptr)
	{
        //_ModelsMgr->resetAnim();

        _Render->r->setRenderBuffer(_Render->rb);
        _Render->r->clear();

        // Camera
        _Pipeline->Clear();
		/*backgroundModel->m_Cameras[0].setup(_EnvironmentManager->globalTime);
		_PipelineGlobal->SetCamera(backgroundModel->m_Cameras[0].GetCamera());
        _PipelineGlobal->SetCameraFrustum(backgroundModel->m_Cameras[0].GetCamera());*/

        _PipelineGlobal->SetCamera(_Render->mainCamera);
        _PipelineGlobal->SetCameraFrustum(_Render->mainCamera);
        
        
        // Geom
		backgroundModel->draw();

        // Postprocess pass
        _Render->r->setRenderBuffer(0);
        for (uint32 i = 0; i < 4; i++) _Render->r->setTexture(i, _Render->r->getRenderBufferTex(_Render->rb, i), 0, R_TextureUsage::Texture);
        _Render->r->clear(CLR_COLOR_RT0 | CLR_DEPTH);

        // Simple pass
        _TechniquesMgr->m_SimpleRender->BindS();
        _TechniquesMgr->m_SimpleRender->SetScreenSize(Modules::config().windowSizeX, Modules::config().windowSizeY);

        _Render->r->setDepthTest(false);
        _Render->r->setBlendMode(true, R_BlendFunc::BS_BLEND_ONE, R_BlendFunc::BS_BLEND_ONE);

        _Render->RenderQuad();

        _Render->r->setBlendMode(false);
        _Render->r->setDepthTest(true);

        _TechniquesMgr->m_SimpleRender->Unbind();
	}
}

void GameState_Menu::RenderUI()
{
	if (cmd == CMD_SELECT2)
	{

		if (_Map->GetMinimap() != 0)
		{
            m_MinimapTexture->SetObj(_Map->GetMinimap());

            m_MinimapUI->Show();
		}
        else
        {
            m_MinimapUI->Hide();
        }
	}   

	if (cmd == CMD_SELECT2)
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
}

//

void GameState_Menu::OnBtn(DBC_MapRecord* _e)
{
    Modules::log().Green("Load level %s [%d]", _e->Get_Directory(), _e->Get_ID());

    _Map->Load_WDT(_e);
    cmd = CMD_SELECT2;

    m_MinimapUI->AttachTo(m_Window);
}

bool GameState_Menu::LoadWorld(cvec3 _pos)
{
	_Map->enterTile(_pos.x / C_TileSize, _pos.z / C_TileSize);

    _Render->mainCamera->Position = _pos;
    _Render->mainCamera->Update();

	if (backgroundModel != nullptr)
	{
		delete backgroundModel;
	}

    // Change GameState
    GameStateManager::SetGameState(GameStatesNames::GAME_STATE_WORLD);

	return true;
}

//

#pragma region Input functional

On_Mouse_Moved(GameState_Menu)
{
    if (enableFreeCamera)
    {
        vec2 mouseDelta = (_mousePos - lastMousePos) / Modules::config().GetWindowSize();

        _Render->mainCamera->ProcessMouseMovement(mouseDelta.x, -mouseDelta.y);

        _Engine->GetAdapter()->SetMousePosition(lastMousePos);
    }
}

On_Mouse_Pressed(GameState_Menu)
{
    /*if (_button == OW_MOUSE_BUTTON_LEFT)
    {
        enableFreeCamera = true;
        lastMousePos = _mousePos;
        _Engine->GetAdapter()->HideCursor();
        return true;
    }*/

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
			pointInWorld = _Map->GetGlobalWMOPlacementInfo()->position;
		}

		delete backgroundModel;
		backgroundModel = nullptr;

		LoadWorld(pointInWorld);

		return true;
	}

	return false;
}

On_Mouse_Released(GameState_Menu)
{

        enableFreeCamera = false;
        lastMousePos = VECTOR_ZERO;
        _Engine->GetAdapter()->ShowCursor();
        return true;


    return false;
}

On_Keyboard_Pressed(GameState_Menu)
{
	if (_key == OW_KEY_ESCAPE)
	{
		if (cmd == CMD_SELECT2)
		{
			cmd = CMD_NONE2;

            m_MinimapUI->Detach();
			//_UIMgr->Attach(window);
		}
		else
		{
			_Engine->Destroy();
		}
	}

	if (_key == OW_KEY_KP_1)
	{
		Modules::config().draw_map_chunk = !Modules::config().draw_map_chunk;
		return true;
	}
	if (_key == OW_KEY_KP_2)
	{
		Modules::config().draw_map_wmo = !Modules::config().draw_map_wmo;
		return true;
	}
	if (_key == OW_KEY_KP_3)
	{
		Modules::config().draw_map_wmo_doodads = !Modules::config().draw_map_wmo_doodads;
		return true;
	}
	if (_key == OW_KEY_KP_4)
	{
		Modules::config().draw_map_mdx = !Modules::config().draw_map_mdx;
		return true;
	}

	if (_key == OW_KEY_C)
	{
		Modules::config().enableMCCV = !Modules::config().enableMCCV;
		return true;
	}

	if (_key == OW_KEY_V)
	{
		Modules::config().enableMCLV = !Modules::config().enableMCLV;
		return true;
	}

	if (_key == OW_KEY_H)
	{
		Modules::config().drawhighres = !Modules::config().drawhighres;
		return true;
	}

	if (_key == OW_KEY_F)
	{
		Modules::config().drawfog = !Modules::config().drawfog;
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
	sprintf_s(path, "Interface\\Glues\\Models\\UI_%s\\UI_%s.m2", randui, randui);

	backgroundModel = new MDX(path);
	backgroundModel->Init();
	backgroundModel->m_IsBillboard = true;
}