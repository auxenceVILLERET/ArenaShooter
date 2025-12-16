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

using namespace gce;

class GameScene : public CustomScene
{
public:
    GameScene(Scene* pScene) : CustomScene(pScene) {}
    
    void Init() override
    {
        CustomScene::Init();

        GameObject& player = AddObject();
        player.transform.SetWorldPosition({ 0,10,0 });
        player.transform.SetWorldScale({ 1.f, 1.f, 1.f });
        player.AddComponent<BoxCollider>();
        player.AddComponent<PhysicComponent>()->SetMass(80.0f);
        player.GetComponent<PhysicComponent>()->SetBounciness(0.0f);
        player.SetName("Player");

        player.AddScript<Player>();
        player.AddScript<PlayerController>();
        
        Vector<GameObject*> temp;
        MapProperties mapProperties = MapLoader::LoadMap(RES_PATH"res/Maps/blockout.json", this, temp);

        std::srand(timeGetTime());
    
        LevelGrid* grid = new LevelGrid();
        grid->Init(SceneName::GAME, mapProperties, {4.0f, 4.0f, 4.0f});
    
        grid->Reset();
    
        GameObject& waveManager = AddObject();
        WaveManager* wManagerComponent = waveManager.AddScript<WaveManager>();
        wManagerComponent->grid = grid;
        wManagerComponent->player = &player;
        wManagerComponent->currScene = this;
        wManagerComponent->spawns = mapProperties.vSpawns;
        wManagerComponent->OnInit();
        pWaveManager = wManagerComponent;
    }

    void Start() override
    {
        CustomScene::Start();
        pWaveManager->OnStart();
    }

private:
    WaveManager* pWaveManager = nullptr;
};


#endif // !MAINMENU_HPP_INCLUDED
