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

ImageUI* uiLvlImage = nullptr;

void Start() override
{
    GameObject& lvl = pScene->AddObject();
    uiLvlImage = lvl.AddComponent<ImageUI>();
    Vector2f32 size2 = { 605, 244 };
    uiLvlImage->InitializeImage({ 20, 50 }, size2, 1.f);
    uiLvlImage->btmBrush = new BitMapBrush("res/ArenaShooter/RDC.png");
    uiLvlImage->btmBrush->SetTransformMatrix({ 20, 50, 0 }, { 0.5f , 0.5f , 0.5f }, 0);


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

    m_vRoomPaths.PushBack(RES_PATH"res/Maps/Room5.json");
}

void Update() override
{
    Door* door = m_pElevator->GetFromScript<Door>();
    if (m_InitOffset == false && initChrono.GetElapsedTime() > 5.0f)
    {
        m_InitOffset = true;
        pPlayer->transform.SetWorldPosition(m_pElevator->roomProperties.playerSpawn);
        return;
    }

    if (m_InitOffset == false)
        return;
    
    if (m_pCurrRoom == nullptr)
        LoadRoom();

    if (door != nullptr)
    {
        if (door->isClosed == false && isFloorFinished == false)
        {
            if (m_pElevator->DistanceFromRoomBorder() > 5.f)
            {
                door->Toggle();
                if (m_pCurrRoom != nullptr && m_pCurrRoom->hasWaves)
                    m_pCurrRoom->pWaveManager->StartWave();
                if (m_pCurrRoom != nullptr && m_pCurrRoom->isBossRoom)
                    m_pCurrRoom->pBossManager->StartBoss();
            }
        }   
    }

    if (m_pCurrRoom != nullptr)
    {
        if (m_pCurrRoom->pWaveManager != nullptr)
            if (m_pCurrRoom->pWaveManager->IsFinished())
                isFloorFinished = true;
    }

    if (door == nullptr)
        return;

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

    if (m_roomIndex == m_vRoomPaths.Size() - 1)
        m_pCurrRoom->isBossRoom = true;
    else
    {
        m_pCurrRoom->hasWaves = true;
        m_pCurrRoom->pWaveManager = pWaveManager;
    m_pCurrRoom->hasWaves = true;
    m_pCurrRoom->pWaveManager = pWaveManager;

    switch (m_roomIndex)
    {
    case 1:
		delete uiLvlImage->btmBrush;
		uiLvlImage->btmBrush = new BitMapBrush("res/ArenaShooter/1er.png");
        uiLvlImage->btmBrush->SetTransformMatrix({ 20, 50, 0 }, { 0.5f , 0.5f , 0.5f }, 0);
        break;
	case 2:
		delete uiLvlImage->btmBrush;
        uiLvlImage->btmBrush = new BitMapBrush("res/ArenaShooter/2eme.png");
		uiLvlImage->btmBrush->SetTransformMatrix({ 20, 50, 0 }, { 0.5f , 0.5f , 0.5f }, 0);
        break;
	case 3:
        delete uiLvlImage->btmBrush;
		uiLvlImage->btmBrush = new BitMapBrush("res/ArenaShooter/3eme.png");
        uiLvlImage->btmBrush->SetTransformMatrix({ 20, 50, 0 }, { 0.5f , 0.5f , 0.5f }, 0);
		break;
	case 4:
		delete uiLvlImage->btmBrush;
        uiLvlImage->btmBrush = new BitMapBrush("res/ArenaShooter/4eme.png");
        uiLvlImage->btmBrush->SetTransformMatrix({ 20, 50, 0 }, { 0.5f , 0.5f , 0.5f }, 0);
		break;
    default:
        break;
    }
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