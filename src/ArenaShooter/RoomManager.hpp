#ifndef ROOM_MANAGER_HPP_INCLUDED
#define ROOM_MANAGER_HPP_INCLUDED

#include "Engine.h"
#include "define.h"
#include "Room.hpp"

DECLARE_SCRIPT(RoomManager, ScriptFlag::Update | ScriptFlag::Start)

void Start() override
{
    
}

void Update() override
{
    
}

private:

Room* m_pElevator = nullptr;
Room* m_pCurrRoom = nullptr;
int8 m_roomIndex = 0;

Vector<Room*> m_vRooms;

END_SCRIPT

#endif