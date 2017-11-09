#include "stdafx.h"

// General
#include "GameStateManager.h"

GameState*                              GameStateManager::currentGameState = nullptr;
map<GameStatesNames::List, GameState*>  GameStateManager::m_GameStatesCollection;

void GameStateManager::AddGameState(GameStatesNames::List _name, GameState* _gameState)
{
    assert1(_gameState != nullptr);
    assert1(m_GameStatesCollection.find(_name) == m_GameStatesCollection.end());

    m_GameStatesCollection.insert(make_pair(_name, _gameState));
}

bool GameStateManager::SetGameState(GameStatesNames::List _name)
{
    assert1(m_GameStatesCollection.find(_name) != m_GameStatesCollection.end());

    GameState* gameState = m_GameStatesCollection[_name];
    return SetGameState(gameState);
}

bool GameStateManager::SetGameState(GameState* _newGameState)
{
    assert1(_newGameState);

    Log::Print("Engine[]: Setting new GameState.");

    // 1. Unset current GameState
    if (currentGameState != nullptr)
    {
        currentGameState->Unset();
        currentGameState->m_IsCurrent = false;
    }

    // 2. If new GameState not inited, init them
    if (!_newGameState->IsInited())
    {
        Log::Warn("Engine[]: New GameState in not inited. Initializating.");
        if (_newGameState->Init())
        {
            _newGameState->m_IsInited = true;
        }
        else
        {
            Log::Error("Engine[]: Error initing new GameState.");
        }
    }

    // 3. Set new GameState
    currentGameState = _newGameState;
    if (currentGameState->Set())
    {
        currentGameState->m_IsCurrent = true;
    }
    else
    {
        Log::Error("Engine[]: Error setting current new GameState.");
    }

    return true;
}