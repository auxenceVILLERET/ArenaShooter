#include "ArenaShot.h"
#include "Player.hpp"
#include "MapLoader.hpp"
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

DECLARE_SCRIPT(CamScript, ScriptFlag::Update)

void Update() override
{
    GameTransform* t = &m_pOwner->transform;
    float32 dt = GameManager::DeltaTime();
    float32 speed = 5.f;
    
    if (GetKey(Keyboard::D))
        t->WorldTranslate(Vector3f32(1.0f, 0.0f, 0.0f) * dt * speed);
    if (GetKey(Keyboard::Q))
        t->WorldTranslate(Vector3f32(-1.0f, 0.0f, 0.0f) * dt * speed);
    if (GetKey(Keyboard::Z))
        t->WorldTranslate(Vector3f32(0.0f, 0.0f, 1.0f) * dt * speed);
    if (GetKey(Keyboard::S))
        t->WorldTranslate(Vector3f32(0.0f, 0.0f, -1.0f) * dt * speed);

    if (GetKey(Keyboard::NUMPAD6))
        t->WorldRotate(Vector3f32(0.0f, 1.0f, 0.0f) * dt * speed);
    if (GetKey(Keyboard::NUMPAD4))
        t->WorldRotate(Vector3f32(0.0f, -1.0f, 0.0f) * dt * speed);
}

END_SCRIPT

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

    MapLoader::LoadMap(RES_PATH"res/Maps/blockout.json", m_Scene, pPso);

    GameObject& player = game->AddObject();
    player.AddScript<Player>();
	player.AddScript<PlayerController>();

    GameObject& kamikaze = game->AddObject();
    kamikaze.AddScript<Kamikaze>();
    kamikaze.AddComponent<BoxCollider>();

}

int Game::RUN()
{
    GameManager::Run(windowParam);

    GameManager::Destroy();

	return 0;
}