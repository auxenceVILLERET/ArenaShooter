#ifndef ROOM_MANAGER_HPP_INCLUDED
#define ROOM_MANAGER_HPP_INCLUDED

#include "Engine.h"
#include "define.h"
#include "Room.hpp"
#include "Thread/Thread.h"

DECLARE_SCRIPT(RoomManager, ScriptFlag::Update | ScriptFlag::Start)

GameObject* pPlayer = nullptr;
CustomScene* pScene = nullptr;
WaveManager* pWaveManager = nullptr;

bool isFloorFinished = false;
bool canSwitchRoom = false;
bool isRoomInit = false;
bool isPlayerLocked = false;

Chrono initChrono;

void Start() override
{
    m_roomIndex = 0;
    isRoomInit = false;
    initChrono.Start();
    
    if (pScene == nullptr)
        return;
    
    GameObject& elevator = pScene->AddObject();
    m_pElevator = elevator.AddScript<Room>();
    m_pElevator->mapPath = RES_PATH"res/Maps/WaitingRoom.json";
    m_pElevator->pPlayer = pPlayer;
    m_pElevator->pScene = pScene;

    GameObject& waveManager = pScene->AddObject();
    pWaveManager = waveManager.AddScript<WaveManager>();
    pWaveManager->player = pPlayer;
    pWaveManager->currScene = pScene;
    pWaveManager->OnInit();

    m_vRoomPaths.PushBack(RES_PATH"res/Maps/Room1.json");
    m_vRoomPaths.PushBack(RES_PATH"res/Maps/Room2.json");
    m_vRoomPaths.PushBack(RES_PATH"res/Maps/Room1.json");
    m_vRoomPaths.PushBack(RES_PATH"res/Maps/Room1.json");
}

void Update() override
{
    if (m_InitOffset == false && initChrono.GetElapsedTime() > 2.0f)
    {
        m_InitOffset = true;
        pPlayer->transform.SetWorldPosition(m_pElevator->roomProperties.playerSpawn);
        return;
    }
    
    if (m_pCurrRoom == nullptr)
        LoadRoom();

    Door* door = m_pElevator->GetFromScript<Door>();
    if (door != nullptr)
    {
        if (door->isClosed == false && isFloorFinished == false)
        {
            if (m_pElevator->DistanceFromRoomBorder() > 5.f)
            {
                door->Toggle();
                if (m_pCurrRoom != nullptr)
                    m_pCurrRoom->pWaveManager->StartWave();
            }
        }   
    }

    if (m_pCurrRoom != nullptr)
    {
        if (m_pCurrRoom->pWaveManager != nullptr)
            if (m_pCurrRoom->pWaveManager->IsFinished())
                isFloorFinished = true;
    }

    if (isFloorFinished)
    {
        if (door->isClosed == true && isPlayerLocked == false)
            door->Toggle();
        
        if (m_pElevator->DistanceFromRoomCenter() < 8.0f & door->isClosed == false)
        {
            door->Toggle();
            isPlayerLocked = true;
        }
        
        if (m_pElevator->DistanceFromRoomCenter() < 6.0f)
            ChangeRoom();
    }

    if (isRoomInit == true)
    {
        door->Toggle();
        isRoomInit = false;
        isFloorFinished = false;
        canSwitchRoom = false;
        isPlayerLocked = false;
    }
}

void LoadRoom()
{
    if (m_roomIndex >= m_vRoomPaths.Size())
        return;
    
    GameObject& temp = pScene->AddObject();
    m_pCurrRoom = temp.AddScript<Room>();
    m_pCurrRoom->mapPath = m_vRoomPaths[m_roomIndex];
    m_pCurrRoom->pPlayer = pPlayer;
    m_pCurrRoom->pScene = pScene;
    m_pCurrRoom->hasLevelGrid = true;
    m_pCurrRoom->hasWaves = true;
    m_pCurrRoom->pWaveManager = pWaveManager;
    m_roomIndex++;
}

void ChangeRoom()
{
    m_pCurrRoom->EmptyRoom();
    m_pCurrRoom = nullptr;

    LoadRoom();

    isRoomInit = true;
}

private:

bool m_InitOffset = false;

Room* m_pElevator = nullptr;
Room* m_pCurrRoom = nullptr;
int8 m_roomIndex = 0;

Vector<String> m_vRoomPaths;

END_SCRIPT

#endif