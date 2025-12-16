#ifndef GAMEMENU_HPP_INCLUDED
#define GAMEMENU_HPP_INCLUDED

#include "define.h"
#include "Engine.h"
#include "SceneManager.h"
#include "CustomScene.h"
#include "MapLoader.hpp"
#include "Player.hpp"
#include "PlayerController.hpp"
#include "Kamikaze.hpp"
#include "LevelGrid.h"
#include "WaveManager.hpp"
#include "Drone.hpp"
#include "Tank.hpp"
#include "EnergyOrb.hpp"

using namespace gce;

class GameScene : public CustomScene
{
public:
    GameScene(Scene* pScene) : CustomScene(pScene) {}
    
    void Init() override
    {
        CustomScene::Init();

        player = &AddObject();
        player->transform.SetWorldPosition({ 0,10,0 });
        player->transform.SetWorldScale({ 1.f, 1.f, 1.f });
        player->AddComponent<BoxCollider>();
        player->AddComponent<PhysicComponent>()->SetMass(80.0f);
        player->GetComponent<PhysicComponent>()->SetBounciness(0.0f);
        player->SetName("Player");
        
        player->AddScript<Player>();
        
        player->AddScript<PlayerController>();
		player->GetScript<Player>()->m_customScene = this;
		player->GetScript<Player>()->Init();

    }

    void Start() override
    {
        CustomScene::Start();

        GameObject& crosshair = AddObject();
        ImageUI& uiImage = *crosshair.AddComponent<ImageUI>();
        Vector2f32 center = { (GameManager::GetWindow()->GetWidth() / 2.f) + 20.f, (GameManager::GetWindow()->GetHeight() / 2.f) + 20.f };
        Vector2f32 size = { 65, 65 };
        Vector2f32 posUi = center - size * 0.5f;
        uiImage.InitializeImage(posUi, size, 1.f);

        uiImage.btmBrush = new BitMapBrush("res/ArenaShooter/crosshair.png");
        uiImage.btmBrush->SetTransformMatrix({ posUi.x, posUi.y, 0 }, { 1.f / 4.f, 1.f / 4.f, 1.f / 4.f }, 0);
        uiImage.SetActive(true);

        MapProperties mapProperties = MapLoader::LoadMap(RES_PATH"res/Maps/blockout.json", this);

        std::srand(timeGetTime());

        LevelGrid* grid = new LevelGrid();
        grid->Init(SceneName::GAME, mapProperties, { 4.0f, 4.0f, 4.0f });

        grid->Reset();

        GameObject& waveManager = AddObject();
        WaveManager* wManagerComponent = waveManager.AddScript<WaveManager>();
        wManagerComponent->grid = grid;
        wManagerComponent->player = player;
        wManagerComponent->currScene = this;
        wManagerComponent->spawns = mapProperties.vSpawns;
        wManagerComponent->OnInit();
        pWaveManager = wManagerComponent;
        pWaveManager->OnStart();
    }

private:
    WaveManager* pWaveManager = nullptr;
    GameObject* player = nullptr;
};


#endif // !MAINMENU_HPP_INCLUDED
