#include "ArenaShot.h"
#include "Player.hpp"
#include "PlayerController.hpp"
#include "Rifle.hpp"
#include "BulletRifle.hpp"
#include "Kamikaze.hpp"
#include "SceneManager.h"
#include "CustomScene.h"

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
    CustomScene* game = m_SceneManager->GetScene(GAME);

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

    GameObject& title = main_menu->AddObject();
    ImageUI& uiImage = *title.AddComponent<ImageUI>();
    Vector2f32 center = { windowParam.width / 2.f, windowParam.height / 2.f };
    Vector2f32 size = { 32, 32 };
    Vector2f32 posUi = center - size * 0.5f;
    uiImage.InitializeImage(posUi, size, 1.f);

    uiImage.btmBrush = new BitMapBrush(RES_PATH"res/ArenaShooter");
    uiImage.btmBrush->SetTransformMatrix({ posUi.x, posUi.y, 0 }, { 1.f / 16.f, 1.f / 16.f, 1.f / 16.f }, 0);
    uiImage.SetActive(true);



    GameObject& player = game->AddObject();
    player.AddScript<Player>()->Init(pPso);
	player.AddScript<PlayerController>();

    GameObject& kamikaze = game->AddObject();
    kamikaze.AddScript<Kamikaze>()->Init(pPso);
    kamikaze.AddComponent<BoxCollider>();


    GameObject& ground = game->AddObject();
    ground.transform.SetWorldPosition({ 0,-3,0 });
    ground.transform.SetWorldScale({ 20.f,5.f,20.f });
    MeshRenderer& meshGround = *ground.AddComponent<MeshRenderer>();
    meshGround.pGeometry = SHAPES.CUBE;
    meshGround.pPso = pPso;
    ground.AddComponent<BoxCollider>();

}

int Game::RUN()
{
    GameManager::Run(windowParam);

    GameManager::Destroy();

	return 0;
}
