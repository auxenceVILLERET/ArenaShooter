#include "ArenaShot.h"
#include "Player.hpp"
#include "MapLoader.hpp"
#include "PlayerController.hpp"
#include "Rifle.hpp"
#include "BulletRifle.hpp"

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
    GameManager::Create();
    m_Scene = &Scene::Create();

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

    MapLoader::LoadMap(RES_PATH"res/Maps/blockout.json", m_Scene, pPso);

    GameObject& player = GameObject::Create(*m_Scene);
    player.AddScript<Player>()->Init(pPso);
	player.AddScript<PlayerController>();

}

int Game::RUN()
{
    GameManager::Run(windowParam);

    GameManager::Destroy();

	return 0;
}