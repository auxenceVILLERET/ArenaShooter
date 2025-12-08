#include "ArenaShot.h"
#include "Player.hpp"
#include "PlayerController.hpp"
#include "Rifle.hpp"
#include "BulletRifle.hpp"
#include "Kamikaze.hpp"

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

    GameObject& player = GameObject::Create(*m_Scene);
    player.AddScript<Player>()->Init(pPso);
	player.AddScript<PlayerController>();

    GameObject& kamikaze = GameObject::Create(*m_Scene);
    kamikaze.AddScript<Kamikaze>()->Init(pPso);
    kamikaze.AddComponent<BoxCollider>();


    GameObject& ground = GameObject::Create(*m_Scene);
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
