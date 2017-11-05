#include "stdafx.h"

// General
#include "GameState_Tests.h"

bool GameState_Tests::Init()
{
    GameState::Init();

    _TechniquesMgr->Init();

    m_ElementTest = new UIElement();
    m_ElementTest->Init(vec2(200, 0), vec2(768, 768), (Texture*)nullptr, COLOR_WHITE);
    m_ElementTest->AttachTo(m_Window);

    UIElement* m_ElementTest1 = new UIElement();
    m_ElementTest1->Init(vec2(20, 20), vec2(100, 100), (Texture*)nullptr, COLOR_GREEN);
    m_ElementTest1->AttachTo(m_ElementTest);

    UIElement* m_ElementTest2 = new UIElement();
    m_ElementTest2->Init(vec2(20, 140), vec2(100, 100), (Texture*)nullptr, COLOR_RED);
    //m_ElementTest2->AttachTo(m_ElementTest);

    UIElement* m_ElementTest3 = new UIElement();
    m_ElementTest3->Init(vec2(140, 20), vec2(100, 100), (Texture*)nullptr, COLOR_GRAY);
    //m_ElementTest3->AttachTo(m_ElementTest);

    //

    return true;
}

//

void GameState_Tests::Input(double t, double dt)
{

}

void GameState_Tests::Update(double t, double dt)
{

}

void GameState_Tests::Render(double t, double dt)
{

}

void GameState_Tests::RenderUI()
{
}

//

#pragma region Input functional

On_Mouse_Moved(GameState_Tests)
{

}

On_Mouse_Pressed(GameState_Tests)
{
	return false;
}

On_Mouse_Released(GameState_Tests)
{
    return false;
}

On_Keyboard_Pressed(GameState_Tests)
{
	return false;
}

#pragma endregion