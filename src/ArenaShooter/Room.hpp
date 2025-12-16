#ifndef ROOM_HPP_INCLUDED
#define ROOM_HPP_INCLUDED

#include "Engine.h"
#include "define.h"
#include "WaveManager.hpp"
#include "LevelGrid.h"

using namespace gce;

DECLARE_SCRIPT(Room, ScriptFlag::Update | ScriptFlag::Start)

WaveManager* pWaveManager = nullptr;
bool hasWaves = false;

LevelGrid* pLevelGrid = nullptr;
bool hasLevelGrid = false;

GameObject* pPlayer = nullptr;
CustomScene* pScene = nullptr;
String mapPath;
MapProperties roomProperties;

void Start() override
{
    roomProperties = MapLoader::LoadMap(mapPath, pScene, m_vObjects);
    
    if ( hasLevelGrid )
    {
        pLevelGrid = new LevelGrid();
        pLevelGrid->Init(pScene, roomProperties, {4.0f, 4.0f, 4.0f});
    }
    
    if ( hasWaves )
    {
        GameObject& waveManager = pScene->AddObject();
        pWaveManager = waveManager.AddScript<WaveManager>();
        pWaveManager->grid = pLevelGrid;
        pWaveManager->player = pPlayer;
        pWaveManager->currScene = pScene;
        pWaveManager->spawns = roomProperties.vSpawns;
        pWaveManager->OnInit();
    }
}

void Update() override
{
    
}

private:

Vector<GameObject*> m_vObjects;

END_SCRIPT

#endif