#include "stdafx.h"

// General
#include "GameState_InWorld.h"

// Additional
#include "World.h"

bool GameState_InWorld::Init()
{
    GameState::Init();

    enableFreeCamera = false;
    cameraSprint = false;

    minimapActive = false;

    return true;
}

void GameState_InWorld::Destroy()
{
    GameState::Destroy();
}

bool GameState_InWorld::Set()
{
    GameState::Set();

    return true;
}

void GameState_InWorld::Unset()
{
    GameState::Unset();
}

void GameState_InWorld::Input(double t, double dt)
{
    float speed = 4.5f;

    if (cameraSlow)
        speed *= 0.2f;

    if (cameraSprint)
        speed *= 3.0f;

    if (Input::IsKeyPressed(OW_KEY_W))
        _Render->mainCamera->ProcessKeyboard(FORWARD, speed);

    if (Input::IsKeyPressed(OW_KEY_S))
        _Render->mainCamera->ProcessKeyboard(BACKWARD, speed);

    if (Input::IsKeyPressed(OW_KEY_A))
        _Render->mainCamera->ProcessKeyboard(LEFT, speed);

    if (Input::IsKeyPressed(OW_KEY_D))
        _Render->mainCamera->ProcessKeyboard(RIGHT, speed);
}

void GameState_InWorld::Update(double t, double dt)
{
    _EnvironmentManager->animtime += (dt * 1000.0f);
    _EnvironmentManager->globalTime = (int)_EnvironmentManager->animtime;

    _World->tick(dt);
}

void GameState_InWorld::Render(double t, double dt)
{
    if (!minimapActive)
    {
        _World->Render();
    }
}

void GameState_InWorld::RenderUI()
{
    //if (_World->loading)
    //{
    //	_Render->RenderText(vec2(_Render->GetWindowSize().x / 2, 200), _World->GetMap()->IsOutOfBounds() ? "Out of bounds" : "Loading...");
    //}

    if (minimapActive)
    {
        int basex = 200;
        int basey = 0;

        if (_Map.GetMinimap() != 0)
        {
            const int len = 768;

            _Render->RenderTexture(vec2(basex, basey), _Map.GetMinimap(), vec2(len, len));

            // Player position
            /*glBegin(GL_LINES);
            float fx, fz;
            fx = basex + _World->mainCamera->Position.x / C_TileSize * 12.0f;
            fz = basey + _World->mainCamera->Position.z / C_TileSize * 12.0f;
            glVertex2f(fx, fz);
            glColor4f(1, 1, 1, 0);
            glVertex2f(fx + 10.0f * cosf(degToRad(_World->mainCamera->Roll)), fz + 10.0f * sinf(degToRad(_World->mainCamera->Roll)));
            glEnd();*/

            return;
        }
    }

    // HUD
        // Skyname
        //char* sn = _World->skies->getSkyName();
        //if(sn)
        //	_Render->RenderText(vec2(200, 0), string(sn));

        // Area and region




        // Area
    DBÑ_AreaTableRecord* areaRecord = nullptr;
    string areaName = "<unknown>";

    areaRecord = DBÑ_AreaTable[_Map.getAreaID()];
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


    //
    // DEBUG
    //
    _Render->RenderTexture(vec2(_Config.windowSizeX * 2.0 / 3.0, _Config.windowSizeY * 2.0 / 3.0), _Render->r->getRenderBufferTex(_World->rb2, 2), vec2(_Config.windowSizeX / 3, _Config.windowSizeY / 3));


    //


    _Render->RenderText(vec2(5, 20), "Area: [" + areaName + "] [Area id = " + std::to_string(_Map.getAreaID()) + "]");
    _Render->RenderText(vec2(5, 40), "Region: [" + regionName + "]");
    _Render->RenderText(vec2(5, 60), "CURRX: " + to_string(_Map.GetCurrentX()) + ", CURRZ " + to_string(_Map.GetCurrentZ()));


    ///

    _Perfomance->Draw(vec2(5, 100));

    _Render->RenderText(vec2(5, _Config.windowSizeY - 66), "REAL CamPos: [" + to_string(_Render->mainCamera->Position.x) + "], [" + to_string(_Render->mainCamera->Position.y) + "], [" + to_string(_Render->mainCamera->Position.z) + "]");
    _Render->RenderText(vec2(5, _Config.windowSizeY - 44), "CamPos: [" + to_string(-(_Render->mainCamera->Position.x - C_ZeroPoint)) + "], [" + to_string(-(_Render->mainCamera->Position.z - C_ZeroPoint)) + "], [" + to_string(_Render->mainCamera->Position.y) + "]");
    _Render->RenderText(vec2(5, _Config.windowSizeY - 22), "CamRot: [" + to_string(_Render->mainCamera->Direction.x) + "], [" + to_string(_Render->mainCamera->Direction.y) + "], [" + to_string(_Render->mainCamera->Direction.z) + "]");

    // Time
    _Render->RenderText(vec2(_Config.windowSizeX - 150, 0), "TIME [" + to_string(_EnvironmentManager->m_GameTime.GetHour()) + "." + to_string(_EnvironmentManager->m_GameTime.GetMinute()) + "]");
    char buff[256];

    // Ambient

    sprintf(buff, "Amb[c=[%0.2f %0.2f %0.2f] i=[%f]]",
    _EnvironmentManager->dayNightPhase.ambientColor.x, _EnvironmentManager->dayNightPhase.ambientColor.y, _EnvironmentManager->dayNightPhase.ambientColor.z,
    _EnvironmentManager->dayNightPhase.ambientIntensity
    );
    _Render->RenderText(vec2(_Config.windowSizeX - 400, 20), buff);

    // Day

    sprintf(buff, "Day[c=[%0.2f %0.2f %0.2f] i=[%f] d=[%0.2f %0.2f %0.2f]]",
    _EnvironmentManager->dayNightPhase.dayColor.x, _EnvironmentManager->dayNightPhase.dayColor.y, _EnvironmentManager->dayNightPhase.dayColor.z,
    _EnvironmentManager->dayNightPhase.dayIntensity,
    _EnvironmentManager->dayNightPhase.dayDir.x, _EnvironmentManager->dayNightPhase.dayDir.y, _EnvironmentManager->dayNightPhase.dayDir.z
    );
    _Render->RenderText(vec2(_Config.windowSizeX - 400, 40), buff);

    // Night

    sprintf(buff, "Nig[c=[%0.2f %0.2f %0.2f] i=[%f] d=[%0.2f %0.2f %0.2f]]\0",
    _EnvironmentManager->dayNightPhase.nightColor.x, _EnvironmentManager->dayNightPhase.nightColor.y, _EnvironmentManager->dayNightPhase.nightColor.z,
    _EnvironmentManager->dayNightPhase.nightIntensity,
    _EnvironmentManager->dayNightPhase.nightDir.x, _EnvironmentManager->dayNightPhase.nightDir.y, _EnvironmentManager->dayNightPhase.nightDir.z
    );
    _Render->RenderText(vec2(_Config.windowSizeX - 400, 60), buff);

    // Fog

    sprintf(buff, "Fog[end=[%f] koeff=[%f]]\0",
            _EnvironmentManager->skies->fogSet[FOG_DISTANCE],
            _EnvironmentManager->skies->fogSet[FOG_MULTIPLIER]
    );
    _Render->RenderText(vec2(_Config.windowSizeX - 400, 80), buff);

    // Colors
    float xPos = _Config.windowSizeX - 400;
    float yPos = 100;

    const char* names[18] =
    {
        "LIGHT_GLOBAL_DIFFUSE" ,
        "LIGHT_GLOBAL_AMBIENT",

        "SKY_COLOR_0",
        "SKY_COLOR_1",
        "SKY_COLOR_2",
        "SKY_COLOR_3",
        "SKY_COLOR_4",

        "FOG_COLOR",

        "SKY_UNKNOWN_1",

        "SUN_COLOR",
        "SUN_HALO_COLOR",

        "SKY_UNKNOWN_2",

        "CLOUD_COLOR",

        "SKY_UNKNOWN_3",

        "OCEAN_COLOR_LIGHT",
        "OCEAN_COLOR_DARK",

        "RIVER_COLOR_LIGHT",
        "RIVER_COLOR_DARK"
    };

    for (uint8 i = 0; i < 18; i++)
    {
        _Render->RenderRectangle(vec2(xPos,      yPos + i * 16), vec2(16.0f, 16.0f), Color(_EnvironmentManager->skies->colorSet[i]));
        _Render->RenderText(     vec2(xPos + 20, yPos + i * 16), names[i]);
    }


    RenderUIDebug();
}

void GameState_InWorld::RenderUIDebug()
{
    char buff[256];
    sprintf(buff, "Buffer memory [%d] bytes", _Render->r->getBufferMem());
    _Render->RenderText(vec2(_Config.windowSizeX - 400, _Config.windowSizeY - 40), buff);

    sprintf(buff, "Texture memory [%d] bytes", _Render->r->getTextureMem());
    _Render->RenderText(vec2(_Config.windowSizeX - 400, _Config.windowSizeY - 20), buff);
}

//

#pragma region Input functional

On_Mouse_Moved(GameState_InWorld)
{
    if (enableFreeCamera)
    {
        vec2 mouseDelta = (_mousePos - lastMousePos) / _Config.GetWindowSize();

        _Render->mainCamera->ProcessMouseMovement(mouseDelta.x, -mouseDelta.y);

        _Engine->GetAdapter()->SetMousePosition(lastMousePos);
    }
}

On_Mouse_Pressed(GameState_InWorld)
{
    if (_button == OW_MOUSE_BUTTON_LEFT)
    {
        enableFreeCamera = true;
        lastMousePos = _mousePos;
        _Engine->GetAdapter()->HideCursor();
        return true;
    }

    return false;
}

On_Mouse_Released(GameState_InWorld)
{
    if (_button == OW_MOUSE_BUTTON_LEFT)
    {
        enableFreeCamera = false;
        lastMousePos = VECTOR_ZERO;
        _Engine->GetAdapter()->ShowCursor();
        return true;
    }

    return false;
}

On_Mouse_WScrolled(GameState_InWorld)
{
    return false;
}

On_Keyboard_Pressed(GameState_InWorld)
{
    if (_key == OW_KEY_ESCAPE)
    {
        GameStateManager::SetGameState(GameStatesNames::GAME_STATE_MENU);
        return true;
    }

    if (_key == OW_KEY_X)
    {
        cameraSprint = true;
        return true;
    }

    if (_key == OW_KEY_Z)
    {
        cameraSlow = true;
        return true;
    }

    if (_key == OW_KEY_KP_1)
    {
        _Config.draw_map_chunk = !_Config.draw_map_chunk;
        return true;
    }
    if (_key == OW_KEY_KP_2)
    {
        _Config.draw_map_wmo = !_Config.draw_map_wmo;
        return true;
    }
    if (_key == OW_KEY_KP_3)
    {
        _Config.draw_map_wmo_doodads = !_Config.draw_map_wmo_doodads;
        return true;
    }

    if (_key == OW_KEY_KP_4)
    {
        _Config.draw_map_mdx = !_Config.draw_map_mdx;
        return true;
    }

    if (_key == OW_KEY_F1)
    {
        _Config.Quality.Texture_Sampler = R_SamplerState::SS_FILTER_POINT;
        return true;
    }

    if (_key == OW_KEY_F2)
    {
        _Config.Quality.Texture_Sampler = R_SamplerState::SS_FILTER_BILINEAR;
        return true;
    }

    if (_key == OW_KEY_F3)
    {
        _Config.Quality.Texture_Sampler = R_SamplerState::SS_FILTER_TRILINEAR;
        return true;
    }

    if (_key == OW_KEY_F6)
    {
        _Config.Quality.Texture_Sampler = R_SamplerState::SS_ANISO1;
        return true;
    }

    if (_key == OW_KEY_F7)
    {
        _Config.Quality.Texture_Sampler = R_SamplerState::SS_ANISO2;
        return true;
    }

    if (_key == OW_KEY_F8)
    {
        _Config.Quality.Texture_Sampler = R_SamplerState::SS_ANISO4;
        return true;
    }

    if (_key == OW_KEY_F9)
    {
        _Config.Quality.Texture_Sampler = R_SamplerState::SS_ANISO8;
        return true;
    }

    if (_key == OW_KEY_F10)
    {
        _Config.Quality.Texture_Sampler = R_SamplerState::SS_ANISO16;
        return true;
    }
   
    

    if (_key == OW_KEY_C)
    {
        _Config.Switch(_Config.Quality.Terrain_MCCV);
        return true;
    }

    if (_key == OW_KEY_V)
    {
        _Config.Switch(_Config.Quality.Terrain_MCLV);
        return true;
    }

    if (_key == OW_KEY_H)
    {
        //_Config.drawhighres = !_Config.drawhighres;
        return true;
    }

    if (_key == OW_KEY_F)
    {
        _Config.drawfog = !_Config.drawfog;
        return true;
    }

    // minimap
    if (_key == OW_KEY_M)
    {
        minimapActive = !minimapActive;
        return true;
    }

    return false;
}

On_Keyboard_Released(GameState_InWorld)
{
    if (_key == OW_KEY_X)
    {
        cameraSprint = false;
        return true;
    }

    if (_key == OW_KEY_Z)
    {
        cameraSlow = false;
        return true;
    }

    return false;
}

#pragma endregion