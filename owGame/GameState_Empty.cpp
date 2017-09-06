#include "stdafx.h"

// General
#include "GameState_Empty.h"

GameState_Empty::GameState_Empty() : GameState() {}

bool GameState_Empty::Init()
{
	inited = true;

	return true;
}

void GameState_Empty::Destroy()
{
	inited = false;
}
