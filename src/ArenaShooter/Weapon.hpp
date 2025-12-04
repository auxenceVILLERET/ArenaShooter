#ifndef WEAPON_HPP_INCLUDED
#define WEAPON_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Chrono.h"
using namespace gce;

DECLARE_SCRIPT(Weapon, ScriptFlag::Start | ScriptFlag::Update)

int8 m_ammo = 0;
int8 m_maxAmmo = 0;

float32 m_reloadCooldown = 0.f;
float32 m_shotCooldown = 0.f;

Chrono m_reloadTimer;
Chrono m_shotTimer;

bool m_isShooting = false;
bool m_isReloading = false;

D12PipelineObject* m_PSO = nullptr;

void Start() override
{
    
}

void Update() override
{
    // Fin de reload
    if (m_isReloading && m_reloadTimer.GetElapsedTime() >= m_reloadCooldown)
    {
        m_isReloading = false;
        m_ammo = m_maxAmmo;
        m_reloadTimer.Pause();
        m_reloadTimer.Reset();
    }

    // Cooldown tir
    if (m_isShooting && m_shotTimer.GetElapsedTime() >= m_shotCooldown)
    {
        EndShot();
    }

}


void BeginShot()
{
    if (m_isShooting) return;
    if (m_isReloading) return;
    if (m_ammo <= 0) return;

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

void Reload()
{
    if (m_isReloading == true) return;
    if (m_ammo == m_maxAmmo)
    {
        std::cout << "Chargeur plein" << std::endl;
        return;
    }
    m_isReloading = true;
    m_reloadTimer.Start();
    std::cout << "Reload" << std::endl;
}


END_SCRIPT

#endif // !WEAPON_HPP_INCLUDED
