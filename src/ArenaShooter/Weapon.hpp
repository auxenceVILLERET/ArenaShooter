#ifndef WEAPON_HPP_INCLUDED
#define WEAPON_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Chrono.h"

DECLARE_SCRIPT(Weapon, ScriptFlag::Start | ScriptFlag::Update)

int8 m_ammo = 0;
int8 m_maxAmmo = 0;

float32 m_reloadCooldown = 0.f;
float32 m_shotCooldown = 0.f;

gce::Chrono m_reloadTimer;
gce::Chrono m_shotTimer;

bool m_isShooting = false;
bool m_isReloading = false;


void Start() override
{
    
}

void Update() override
{
    if (m_ammo <= 0)
    {
        m_isReloading = true;
    }

    if (m_isReloading == true)
    {
        if (m_reloadTimer.GetElapsedTime() >= m_reloadCooldown)
        {
            m_ammo = m_maxAmmo;
            m_isReloading = false;
            m_reloadTimer.Pause();
            m_reloadTimer.Reset();
        }
        return;
    }

    
    if (m_shotTimer.GetElapsedTime() < m_shotCooldown)
        return;


    if (m_isShooting)
        Shoot();


}


virtual void BeginShot()
{
    if (m_isReloading) return;
    if (m_ammo <= 0) return;

    m_isShooting = true;
}

virtual void Shoot()
{
    
}

virtual void EndShot()
{
    m_isShooting = false;
    m_shotTimer.Pause();
    m_shotTimer.Reset();
}

void Reload()
{
    if (m_isReloading) return;

    m_isReloading = true;
    m_reloadTimer.Start();
}

END_SCRIPT

#endif // !WEAPON_HPP_INCLUDED
