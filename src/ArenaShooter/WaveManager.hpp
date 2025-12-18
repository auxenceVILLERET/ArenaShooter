#ifndef WAVE_MANAGER_HPP_INCLUDED
#define WAVE_MANAGER_HPP_INCLUDED

#include "define.h"
#include "MapLoader.hpp"
#include "Script.h"
#include "Utils.h"
#include "Event.hpp"
#include "Kamikaze.hpp"
#include "Tank.hpp"
#include "Drone.hpp"
#include "Player.hpp"

enum EnemyCost
{
    KAMIKAZE = 1,
    DRONE = 2,
    TANK = 3
};

DECLARE_SCRIPT(WaveManager, ScriptFlag::Update)

int8 maxWaveCount = 3;
int8 floorFactor = 0;
int8 waveValue = 0;
int8 remainingWaveValue = 0;

int8 currentWave = 0;
bool isSpawningWave = false;
bool isFightingWave = false;
bool isReady = false;

Vector<Spawn> spawns;

Chrono waveSpawnChrono;
float32 waveSpawnDelay = 0.5f;

Chrono waveIntervalChrono;
float32 waveInterval = 1.5f;

Event<WaveManager*> onEnemyDeathEvent;

CustomScene* currScene = nullptr;
GameObject* player = nullptr;
LevelGrid* grid = nullptr;

Vector<Enemy*> vEnemy;

void OnInit() 
{
    Geometry* pKamikazeGeo = GeometryFactory::LoadGeometry(RES_PATH"res/ArenaShooter/Obj/Kamikaze.obj");
    Texture* albedoKamikaze = new Texture(RES_PATH"res/ArenaShooter/Obj/Kamikaze_color.png");
    Texture* roughKamikaze = new Texture(RES_PATH"res/ArenaShooter/Obj/Kamikaze_rough.png");
    Texture* aoKamikaze = new Texture(RES_PATH"res/ArenaShooter/Obj/Kamikaze_ao.png");

    Geometry* pTankGeo = GeometryFactory::LoadGeometry(RES_PATH"res/ArenaShooter/Obj/tank.obj");
    Texture* albedoTank = new Texture(RES_PATH"res/ArenaShooter/Obj/tank_color.png");
    Texture* normTank = new Texture(RES_PATH"res/ArenaShooter/Obj/tank_norm.png");
    Texture* roughTank = new Texture(RES_PATH"res/ArenaShooter/Obj/tank_rough.png");
    Texture* aoTank = new Texture(RES_PATH"res/ArenaShooter/Obj/tank_ao.png");


    for (int i = 0; i < 20; i++)
    {
        GameObject* newEnemy = &currScene->AddObject();
        MeshRenderer& mesh = *newEnemy->AddComponent<MeshRenderer>();
        mesh.pGeometry = pKamikazeGeo;
		mesh.pMaterial->albedoTextureID = albedoKamikaze->GetTextureID();
		mesh.pMaterial->useTextureAlbedo = 1;
        mesh.pMaterial->roughnessTextureID = roughKamikaze->GetTextureID();
        mesh.pMaterial->useTextureRoughness = 1;
        mesh.pMaterial->ambientTextureID = aoKamikaze->GetTextureID();
        mesh.pMaterial->useTextureAmbient = 1;
        newEnemy->transform.SetWorldScale({ 1.3f,1.3f,1.3f });

        newEnemy->SetName("Kamikaze");
        Kamikaze* tempScript = newEnemy->AddScript<Kamikaze>();
        tempScript->SetGrid(grid);
        tempScript->SetPlayer(player);
        tempScript->m_pCustomScene = currScene;
        newEnemy->AddComponent<BoxCollider>();
        PhysicComponent* newEnemyPC = newEnemy->AddComponent<PhysicComponent>();
        newEnemyPC->SetGravityScale(0.0f);
        newEnemyPC->SetIsTrigger(true);
        newEnemy->SetActive(false);

        Enemy* enemyScript = dynamic_cast<Enemy*>(tempScript);
        vEnemy.PushBack(enemyScript);
    }

    for (int i = 0; i < 20; i++)
    {
        GameObject* newEnemy = &currScene->AddObject();
        MeshRenderer& mesh = *newEnemy->AddComponent<MeshRenderer>();
        mesh.pGeometry = pTankGeo;
        mesh.pMaterial->albedoTextureID = albedoTank->GetTextureID();
        mesh.pMaterial->useTextureAlbedo = 1;
		mesh.pMaterial->normalTextureID = normTank->GetTextureID();
		mesh.pMaterial->useTextureNormal = 1;
		mesh.pMaterial->roughnessTextureID = roughTank->GetTextureID();
		mesh.pMaterial->useTextureRoughness = 1;
		mesh.pMaterial->ambientTextureID = aoTank->GetTextureID();
		mesh.pMaterial->useTextureAmbient = 1;
        newEnemy->transform.SetWorldScale({ 1.3f,1.3f,1.3f });

        newEnemy->SetName("Tank");
        Tank* tempScript = newEnemy->AddScript<Tank>();
        tempScript->SetGrid(grid);
        tempScript->SetPlayer(player);
        tempScript->m_pCustomScene = currScene;
        newEnemy->AddComponent<BoxCollider>();
        PhysicComponent* newEnemyPC = newEnemy->AddComponent<PhysicComponent>();
        newEnemyPC->SetGravityScale(0.0f);
        newEnemyPC->SetIsTrigger(true);
        newEnemy->SetActive(false);

        Enemy* enemyScript = dynamic_cast<Enemy*>(tempScript);
        vEnemy.PushBack(enemyScript);
    }


     for (int i = 0; i < 20; i++)
     {
         GameObject* newEnemy = &currScene->AddObject();
         MeshRenderer& mesh = *newEnemy->AddComponent<MeshRenderer>();
         mesh.pGeometry = SHAPES.CUBE;
         /*mesh.pMaterial->albedoTextureID = albedoKamikaze->GetTextureID();
         mesh.pMaterial->useTextureAlbedo = 1;*/
         newEnemy->transform.SetWorldScale({ 1.3f,1.3f,1.3f });
    
         newEnemy->SetName("Drone");
         Drone* tempScript = newEnemy->AddScript<Drone>();
         tempScript->SetGrid(grid);
         tempScript->SetPlayer(player);
         tempScript->m_pCustomScene = currScene;
         newEnemy->AddComponent<BoxCollider>();
         PhysicComponent* newEnemyPC = newEnemy->AddComponent<PhysicComponent>();
         newEnemyPC->SetGravityScale(0.0f);
         newEnemyPC->SetIsTrigger(true);
         newEnemy->SetActive(false);
    
         Enemy* enemyScript = dynamic_cast<Enemy*>(tempScript);
         vEnemy.PushBack(enemyScript);
     }

 

    waveIntervalChrono.Start();
}

void UpdateGrid()
{
    for (Enemy* pEnemy :vEnemy)
    {
        pEnemy->m_pLevelGrid = grid;
    }
}

int8 GetEnemyCount()
{
    int8 count = 0;
    for (Enemy* pEnemy : vEnemy)
    {
        if (pEnemy->m_pOwner->IsActive() == true)
            count++;
    }
    return count;
}

bool IsFinished()
{
    return currentWave >= maxWaveCount && GetEnemyCount() == 0 && !isSpawningWave;
}

template <typename T>
T* GetFirstAvailableEnemy()
{
    for (Enemy* pEnemy : vEnemy)
    {
        if (pEnemy->m_pOwner->IsActive() == true) continue;
        T* pResult = pEnemy->m_pOwner->GetScript<T>();
        if (pResult == nullptr) continue;

        return pResult;
    }

    return nullptr;
}


void StartWave()
{
    if (currentWave > maxWaveCount)
        return;
    
    currentWave++;
    // waveValue = 5 + currentWave * 3 + floorFactor * 3;
    waveValue = 2;
    remainingWaveValue = waveValue;
    isSpawningWave = true;
    isReady = true;
}

void SpawnEnemy(Spawn selectedSpawn)
{
    if (currScene == nullptr)
    {
        std::cerr << "Unvalid Parameters : Unset scene for wave creation" << std::endl;
        return;
    }
    
    Vector<EnemyCost> options;

    //if (remainingWaveValue >= KAMIKAZE) options.PushBack(KAMIKAZE);
    if (remainingWaveValue >= DRONE) options.PushBack(DRONE);
    //if (remainingWaveValue >= TANK) options.PushBack(TANK);

    EnemyCost chosenEnemy = RandomFrom(options);
    
    if (chosenEnemy == KAMIKAZE)
    {
        Kamikaze* tempScript = GetFirstAvailableEnemy<Kamikaze>();
        if (tempScript == nullptr) return;
        tempScript->m_pOwner->transform.SetWorldPosition(selectedSpawn.startPos);
        tempScript->GoToPosition(selectedSpawn.endPos, tempScript->m_speed);
        tempScript->m_pOwner->SetActive(true);
        tempScript->m_Hp->Heal();
        Console::Log("Spawned Kamikaze");
        remainingWaveValue -= KAMIKAZE;
    }

    else if (chosenEnemy == DRONE)
    {
        Drone* tempScript = GetFirstAvailableEnemy<Drone>();
        if (tempScript == nullptr) return;
        tempScript->m_pOwner->transform.SetWorldPosition(selectedSpawn.startPos);
        tempScript->GoToPosition(selectedSpawn.endPos, tempScript->m_speed);
        tempScript->m_pOwner->SetActive(true);
        tempScript->m_Hp->Heal();
        Console::Log("Spawned Drone");
        remainingWaveValue -= DRONE;
    }

    else if (chosenEnemy == TANK) 
    {
        Tank* tempScript = GetFirstAvailableEnemy<Tank>();
        if (tempScript == nullptr) return;
        tempScript->m_pOwner->transform.SetWorldPosition(selectedSpawn.startPos);
        tempScript->GoToPosition(selectedSpawn.endPos, tempScript->m_speed);
        tempScript->m_pOwner->SetActive(true);
        tempScript->m_Hp->Heal();
        Console::Log("Spawned Tank");
		remainingWaveValue -= TANK;
    }
    
}

void SpawnSubWave()
{
    if (waveSpawnChrono.GetElapsedTime() < waveSpawnDelay)
        return;

    waveSpawnChrono.Reset();
    
    for (Spawn currSpawn : spawns)
        SpawnEnemy(currSpawn);
}

void Update() override
{
    if (remainingWaveValue > 0 && isSpawningWave)
        SpawnSubWave();
    else if (isSpawningWave)
    {
        isSpawningWave = false;
        isFightingWave = true;
    }

    if (isFightingWave)
    {
        if (GetEnemyCount() == 0)
        {
            isFightingWave = false;
            waveIntervalChrono.Reset();
            waveIntervalChrono.Start();
        }
    }

    if (isFightingWave == false && isSpawningWave == false)
    {
        if (waveIntervalChrono.GetElapsedTime() > waveInterval)
        {
            if (currentWave < maxWaveCount && isReady == true)
                StartWave();   
        }
    }
    
}

END_SCRIPT

#endif