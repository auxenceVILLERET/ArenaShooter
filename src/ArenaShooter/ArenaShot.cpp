#include "ArenaShot.h"
#include "CustomScene.h"
#include "MainMenu.hpp"
#include "GameMenu.hpp"
#include "OptionsMenu.hpp"

Game* Game::Create()
{
    if (game_Instance != nullptr)
        return game_Instance;

    game_Instance = new Game();
    return game_Instance;
}

Game::Game() {}

void Game::Init()
{
    m_SceneManager = SceneManager::GetInstance();
    
    m_SceneManager->Init();

    CustomScene* main_menu = m_SceneManager->GetScene(MAIN_MENU);
    CustomScene* game_menu = m_SceneManager->GetScene(GAME);
    CustomScene* option_menu = m_SceneManager->GetScene(OPTIONS_MENU);
    
    SceneManager::GetInstance()->ChangeScene(MAIN_MENU);

    pPso = new D12PipelineObject(
        SHADERS.VERTEX,
        SHADERS.PIXEL,
        SHADERS.HULL,
        SHADERS.DOMAIN_,
        SHADERS.ROOT_SIGNATURE
    );

    Console::Log("WindowParam Create", Console::BLUE);
    windowParam.title = L"Arena Game";
    windowParam.width = 1080;
    windowParam.height = 720;
    windowParam.isFullScreen = true;
}

int Game::RUN()
{
    GameManager::Run(windowParam);

    GameManager::Destroy();

	return 0;
}