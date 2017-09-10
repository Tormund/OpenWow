#include "stdafx.h"

// General
#include "GameState_Empty.h"

GameState_Empty::GameState_Empty() : GameState() {}

bool GameState_Empty::Init()
{
	m_inited = true;

	return true;
}

void GameState_Empty::Destroy()
{
	m_inited = false;
}
