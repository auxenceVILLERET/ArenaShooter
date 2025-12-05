#include "ArenaShot.h"
#include "Player.hpp"
#include "MapLoader.hpp"

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

    p_Pso = new D12PipelineObject(
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

    // GameObject& player = GameObject::Create(*m_Scene);
    // player.AddScript<PlayerMovement>();
    //
    // MeshRenderer& meshPlayer = *player.AddComponent<MeshRenderer>();
    // meshPlayer.pGeometry = SHAPES.CUBE;
    // meshPlayer.pPso = p_Pso;
    //
    // GameObject& ground = GameObject::Create(*m_Scene);
    // ground.transform.SetWorldPosition({ 0,-3,0 });
    // ground.transform.SetWorldScale({ 5,5,5 });
    // MeshRenderer& meshGround = *ground.AddComponent<MeshRenderer>();
    // meshGround.pGeometry = SHAPES.CUBE;
    // meshGround.pPso = p_Pso;
    // ground.AddComponent<BoxCollider>();

    MapLoader::LoadMap(RES_PATH"res/Maps/blockout.json", m_Scene, p_Pso);
    
    GameObject& cam = GameObject::Create(*m_Scene);
    cam.transform.SetWorldPosition({ 0,2,-8 });

    Camera& camera = *cam.AddComponent<Camera>();
    camera.SetMainCamera();
    camera.SetType(PERSPECTIVE);
    camera.perspective.fov = XM_PIDIV4;
    camera.perspective.nearPlane = 0.001f;
    camera.perspective.farPlane = 500.0f;
    camera.perspective.aspectRatio = 600.0f / 400.0f;
    camera.perspective.up = { 0.0f, 1.0f, 0.0f };
    cam.AddScript<CamScript>();

}

int Game::RUN()
{
    GameManager::Run(windowParam);

    GameManager::Destroy();

	return 0;
}