#ifndef WEAPON_HPP_INCLUDED
#define WEAPON_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Chrono.h"
#include "Projectile.hpp"
using namespace gce;

DECLARE_SCRIPT(Weapon, ScriptFlag::Start | ScriptFlag::Update)


float32 m_reloadCooldown = 0.f;
float32 m_shotCooldown = 0.f;

Chrono m_reloadTimer;
Chrono m_shotTimer;

bool m_isShooting = false;
bool m_isReloading = false;

D12PipelineObject* m_PSO = nullptr;

float32 m_heat = 0.f;          
float32 m_maxHeat = 100.f;     
float32 m_heatPerShot = 10.f;  
float32 m_coolRate = 8.f;     

bool m_isOverheated = false;

Vector<Projectile*> m_pProjectiles;

void Start() override
{
    
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

    // Fin de reload
    if (m_isReloading && m_reloadTimer.GetElapsedTime() >= m_reloadCooldown)
    {
        m_isReloading = false;
        m_heat = 0.f;
        m_reloadTimer.Pause();
        m_reloadTimer.Reset();
    }

    // Cooldown tir
    if (m_isShooting && m_shotTimer.GetElapsedTime() >= m_shotCooldown && m_heat <= m_maxHeat && m_isOverheated == false)
    {
        EndShot();
    }

    if (m_heat <= 0.f)
    {
        m_heat = 0.f;
    }

}

void BeginShot()
{
    if (m_isShooting) return;
    if (m_isReloading) return;

    m_isShooting = true;

    Shoot();  
}

virtual void Shoot()
{
    
}

virtual void Init(D12PipelineObject* pso)
{

}

void EndShot()
{
    m_isShooting = false;
    m_shotTimer.Pause();
    m_shotTimer.Reset();
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

void Reload()
{
    if (m_isReloading == true) return;

    m_isReloading = true;
    m_reloadTimer.Start();
    std::cout << "Reload" << std::endl;
}


END_SCRIPT

#endif // !WEAPON_HPP_INCLUDED
