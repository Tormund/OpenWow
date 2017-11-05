#pragma once

#include "GameState.h"

struct GameStatesNames
{
    enum List
    {
        GAME_STATE_MENU,
        GAME_STATE_WORLD
    };
};

class GameStateManager
{
public:
    static void AddGameState(GameStatesNames::List _name, GameState* _gameState);

    static bool SetGameState(GameStatesNames::List _name);
    static bool SetGameState(GameState* _newGameState);

    static GameState* GetGameState() { return currentGameState; }

private:
    static GameState* currentGameState;
    static map<GameStatesNames::List, GameState*> m_GameStatesCollection;
};