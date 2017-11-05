#include "stdafx.h"

// General
#include "GameState.h"

GameState::GameState() : m_IsInited(false), m_IsCurrent(false)
{}

bool GameState::Init()
{
    m_Window = new UIWindow();
    m_Window->Init(vec2(0.0f, 0.0f), vec2(Modules::config().windowSizeX, Modules::config().windowSizeY), nullptr);

    m_IsInited = true;

    return true;
}

void GameState::Destroy()
{}

bool GameState::Set()
{
    UpdatableObject::Register();
    RenderableUIObject::Register(100);
    InputListenerObject::Register();

    _UIMgr->SetRootElement(m_Window);

    return true;
}

void GameState::Unset()
{
    //m_Window->Detach();

    UpdatableObject::Unregister();
    RenderableUIObject::Unregister();
    InputListenerObject::Unregister();
}
