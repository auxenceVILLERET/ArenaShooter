#ifndef ROOM_MANAGER_HPP_INCLUDED
#define ROOM_MANAGER_HPP_INCLUDED

#include "Engine.h"
#include "define.h"
#include "Room.hpp"

DECLARE_SCRIPT(RoomManager, ScriptFlag::Update | ScriptFlag::Start)

GameObject* pPlayer = nullptr;
CustomScene* pScene = nullptr;

void Start() override
{
    if (pScene == nullptr)
        return;

    GameObject& elevator = pScene->AddObject();
    m_pElevator = elevator.AddScript<Room>();
    m_pElevator->mapPath = RES_PATH"res/Maps/WaitingRoom.json";
    m_pElevator->pPlayer = pPlayer;
    m_pElevator->pScene = pScene;
}

void Update() override
{
    
}

private:

Room* m_pElevator = nullptr;
Room* m_pCurrRoom = nullptr;
int8 m_roomIndex = 0;

Vector<String> m_vRoomPaths;

END_SCRIPT

#endif