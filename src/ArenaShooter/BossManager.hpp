#ifndef BOSS_MANAGER_HPP_INCLUDED
#define BOSS_MANAGER_HPP_INCLUDED

#include "define.h"
#include "MapLoader.hpp"
#include "Script.h"
#include "Utils.h"
#include "Event.hpp"
#include "Boss.hpp"
#include "Kamikaze.hpp"
#include "Tank.hpp"
#include "Player.hpp"



DECLARE_SCRIPT(BossManager, ScriptFlag::Update)

int8 maxWaveCount = 3;
int8 floorFactor = 0;
int8 waveValue = 0;
int8 remainingWaveValue = 0;

bool bossAlreadySpawned = false;
bool isSpawningBoss = false;
bool isFightingBoss = false;

int8 currentWave = 0;
bool isSpawningWave = false;
bool isFightingWave = false;

bool kamikazeFirstPhaseStarted = false;
bool kamikazeSecondPhaseStarted = false;

Vector<Spawn> spawns;

Spawn BossSpawn = { { 35.f,4.f,0.f }, { 25.f,4.f,0.f } };
Spawn KamikazeSpawn1 = { { 25.f,4.f,0.f }, { 25.f,8.f,-4.f } };
Spawn KamikazeSpawn2 = { { 25.f,4.f,0.f }, { 25.f,8.f,4.f } };


Chrono waveSpawnChrono;
float32 waveSpawnDelay = 10.f;

Event<BossManager*> onEnemyDeathEvent;

CustomScene* currScene = nullptr;
GameObject* player = nullptr;
LevelGrid* grid = nullptr;

Enemy* boss;
Vector<Enemy*> vEnemy;

void OnInit() 
{
    GameObject* newEnemy = &currScene->AddObject();
    MeshRenderer& mesh = *newEnemy->AddComponent<MeshRenderer>();
    mesh.pGeometry = SHAPES.CUBE;
    newEnemy->transform.SetWorldScale({ 1.f,1.f,1.f });
    
    Boss* tempScript = newEnemy->AddScript<Boss>();
    tempScript->SetGrid(grid);
    tempScript->SetPlayer(player);

    newEnemy->AddComponent<BoxCollider>(); 
    PhysicComponent* newEnemyPC = newEnemy->AddComponent<PhysicComponent>();
    newEnemyPC->SetGravityScale(0.0f);
    newEnemyPC->SetIsTrigger(true);
    newEnemy->SetActive(false);

    boss = dynamic_cast<Enemy*>(tempScript);

    for (int i = 0; i < 20; i++)
    {
        GameObject* newEnemy = &currScene->AddObject();
        MeshRenderer& mesh = *newEnemy->AddComponent<MeshRenderer>();
        mesh.pGeometry = SHAPES.CUBE;
        newEnemy->transform.SetWorldScale({ 1.f,1.f,1.f });

        Kamikaze* tempScript = newEnemy->AddScript<Kamikaze>();
        tempScript->SetGrid(grid);
        tempScript->SetPlayer(player);

        newEnemy->AddComponent<BoxCollider>();
        PhysicComponent* newEnemyPC = newEnemy->AddComponent<PhysicComponent>();
        newEnemyPC->SetGravityScale(0.0f);
        newEnemyPC->SetIsTrigger(true);
        newEnemy->SetActive(false);

        Enemy* enemyScript = dynamic_cast<Enemy*>(tempScript);
        vEnemy.PushBack(enemyScript);
    }
}

void OnStart()
{
    boss->m_pOwner->SetActive(false);
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

void BossSpawned()
{
    bossAlreadySpawned = true;
}

void StartBoss()
{
    isSpawningBoss = true;
}

void SpawnBoss(Spawn selectedSpawn)
{
    if (currScene == nullptr)
    {
        std::cerr << "Unvalid Parameters : Unset scene for boss creation" << std::endl;
        return;
    }

    BossSpawned();
    boss->m_pOwner->transform.SetWorldPosition(selectedSpawn.startPos);
    boss->GoToPosition(selectedSpawn.endPos, boss->m_speed);
    boss->m_pOwner->SetActive(true);
    Console::Log("Spawned Boss");
    
    
}

void StartWave()
{
    isSpawningWave = true;
}

void SpawnEnemy(Spawn selectedSpawn)
{
    if (currScene == nullptr)
    {
        std::cerr << "Unvalid Parameters : Unset scene for wave creation" << std::endl;
        return;
    }

    Kamikaze* tempScript = GetFirstAvailableEnemy<Kamikaze>();
    if (tempScript == nullptr) return;
    tempScript->m_pOwner->transform.SetWorldPosition(selectedSpawn.startPos);
    tempScript->GoToPosition(selectedSpawn.endPos, tempScript->m_speed);
    tempScript->m_pOwner->SetActive(true);
    Console::Log("Spawned Kamikaze");

}

void SpawnSubWave()
{
    if (waveSpawnChrono.GetElapsedTime() < waveSpawnDelay)
        return;

    waveSpawnChrono.Reset();

    SpawnEnemy(KamikazeSpawn1);
    SpawnEnemy(KamikazeSpawn2);
}

void Update() override
{
    if (bossAlreadySpawned == false && isSpawningBoss == true)
        SpawnBoss(BossSpawn);
    else if (isSpawningBoss == true)
    {
        isSpawningBoss = false;
        isFightingBoss = true;
    }

    if (isFightingBoss)
    {
        if (boss == nullptr)
        {
            isFightingBoss = false;
        }
    }

    if (boss->m_Hp->GetHealth() <= 200 && !kamikazeFirstPhaseStarted)
    {
        kamikazeFirstPhaseStarted = true;
    }
    if (boss->m_Hp->GetHealth() <= 100 && !kamikazeSecondPhaseStarted)
    {
        kamikazeSecondPhaseStarted = true;
        waveSpawnChrono.Reset();
    }



    if (kamikazeFirstPhaseStarted)
    {
        SpawnSubWave();
    }

    if (isFightingBoss == false && isSpawningBoss == false)
    {
        StartBoss();
    }
    
}

END_SCRIPT

#endif