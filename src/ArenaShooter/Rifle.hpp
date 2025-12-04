#ifndef RIFLE_HPP_INCLUDED
#define RIFLE_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Chrono.h"
#include "Weapon.hpp"


DECLARE_CHILD_SCRIPT(Rifle, Weapon, ScriptFlag::Start | ScriptFlag::Update)

void Start() override
{
    m_maxAmmo = 25;
    m_ammo = 25;
    m_shotCooldown = 0.1f;
    m_reloadCooldown = 1.5f;
}

void Shoot() override
{
    m_ammo -= 1;
    m_shotTimer.Start();
    std::cout << "test" << std::endl;
}

END_SCRIPT

#endif // !RIFLE_HPP_INCLUDED