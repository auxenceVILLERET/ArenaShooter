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
        MapProperties mapProperties = MapLoader::LoadMap(RES_PATH"res/Maps/blockout.json", this);

        std::srand(timeGetTime());
    
        LevelGrid* grid = new LevelGrid();
        grid->Init(SceneName::GAME, mapProperties, {4.0f, 4.0f, 4.0f});
    
        grid->Reset();
    
        GameObject& player = AddObject();
        player.transform.SetWorldPosition({ 0,10,0 });
        player.transform.SetWorldScale({ 1.f, 1.f, 1.f });
        player.AddComponent<BoxCollider>();
        player.AddComponent<PhysicComponent>()->SetMass(80.0f);
        player.GetComponent<PhysicComponent>()->SetBounciness(0.0f);
        player.SetName("Player");

        player.AddScript<Player>();
        player.AddScript<PlayerController>();
    
        GameObject& waveManager = AddObject();
        WaveManager* wManagerComponent = waveManager.AddScript<WaveManager>();
        wManagerComponent->grid = grid;
        wManagerComponent->player = &player;
        wManagerComponent->currScene = this;
        wManagerComponent->spawns = mapProperties.vSpawns;
        wManagerComponent->OnInit();
        pWaveManager = wManagerComponent;

        GameObject& kamikaze = AddObject();
        MeshRenderer& meshKa = *kamikaze.AddComponent<MeshRenderer>();
        meshKa.pGeometry = SHAPES.CUBE;
        kamikaze.transform.SetWorldPosition({ 35.f,5.f,0.f });
        kamikaze.transform.SetWorldScale({ 1.f,1.f,1.f });
        kamikaze.AddScript<Kamikaze>();
        kamikaze.AddComponent<BoxCollider>();
        PhysicComponent* kamikazePC = kamikaze.AddComponent<PhysicComponent>();
        kamikazePC->SetGravityScale(0.0f);
        kamikazePC->SetIsTrigger(true);
        
        kamikaze.GetScript<Kamikaze>()->SetPlayer(&player);
        kamikaze.GetScript<Kamikaze>()->SetGrid(grid);


        GameObject& drone = AddObject();
        MeshRenderer& meshD = *drone.AddComponent<MeshRenderer>();
        meshD.pGeometry = SHAPES.CUBE;
        drone.transform.SetWorldPosition({ -10.f,5.f,0.f });
        drone.transform.SetWorldScale({ 1.f,1.f,1.f });
        drone.AddScript<Drone>();
        drone.AddComponent<BoxCollider>();
        PhysicComponent* dronePC = drone.AddComponent<PhysicComponent>();
        dronePC->SetGravityScale(0.0f);
        dronePC->SetIsTrigger(true);

        drone.GetScript<Drone>()->SetPlayer(&player);
        drone.GetScript<Drone>()->SetGrid(grid);

        GameObject& tank = AddObject();
        MeshRenderer& meshT = *tank.AddComponent<MeshRenderer>();
        meshT.pGeometry = SHAPES.CUBE;
        tank.transform.SetWorldPosition({ -35.f,2.f,0.f });
        tank.transform.SetWorldScale({ 1.f,1.f,1.f });
        tank.AddScript<Tank>();
        tank.AddComponent<BoxCollider>();
        PhysicComponent* tankPC = tank.AddComponent<PhysicComponent>();
        tankPC->SetGravityScale(0.0f);
        tankPC->SetIsTrigger(true);

        tank.GetScript<Tank>()->SetPlayer(&player);
        tank.GetScript<Tank>()->SetGrid(grid);
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
