#ifndef WEAPON_HPP_INCLUDED
#define WEAPON_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Chrono.h"
#include "Projectile.hpp"

using namespace gce;

class WeaponController;

DECLARE_SCRIPT(Weapon, ScriptFlag::Awake | ScriptFlag::Update)


float32 m_shotCooldown = 0.f;
WeaponController* m_pWeaponController = nullptr;

Chrono m_shotTimer;

bool m_isShooting = false;

D12PipelineObject* m_PSO = nullptr;

float32 m_heat = 0.f;          
float32 m_maxHeat = 100.f;     
float32 m_heatPerShot = 10.f;  
float32 m_coolRate = 8.f;     

bool m_isOverheated = false;

Vector<Projectile*> m_pProjectiles;

GameObject* m_Owner = nullptr;

void Awake() override
{
    m_Owner = m_pOwner;
}

void Update() override
{
    float32 dt = GameManager::DeltaTime();

    if (m_heat >= m_maxHeat)
    {
        m_isOverheated = true;
    }

    if (m_isShooting == false && m_isOverheated == false)
    {
        m_heat -= m_coolRate * dt + 2.f;
    }

    if (m_isOverheated == true)
    {
        m_heat -= m_coolRate * dt;
        if (m_heat <= 30.f)
        {
            m_isOverheated = false;
            return;
        }
    }


    // Cooldown tir
    if (m_isShooting && m_shotTimer.GetElapsedTime() >= m_shotCooldown && m_heat <= m_maxHeat && m_isOverheated == false)
    {
        m_isShooting = false;
        m_shotTimer.Pause();
        m_shotTimer.Reset();
    }

    if (m_heat <= 0.f)
    {
        m_heat = 0.f;
    }

}

virtual void BeginShot()
{
      
}

virtual bool Shoot()
{
    if (m_isShooting) return false;

    m_isShooting = true;

    m_shotTimer.Reset();
    m_shotTimer.Start();
    m_heat += m_heatPerShot;

}

virtual void EndShot()
{
    
}

virtual Projectile* GetFirstAvailableProjectile()
{
    for (Projectile* projectile : m_pProjectiles)
    {
        if (!projectile->IsActive())
            return projectile;
    }

    return nullptr;
}

void SetWeaponController(WeaponController* controller)
{
    m_pWeaponController = controller;
}

GameObject* GetOwner()
{
    return m_Owner;
}

END_SCRIPT

#endif // !WEAPON_HPP_INCLUDED
