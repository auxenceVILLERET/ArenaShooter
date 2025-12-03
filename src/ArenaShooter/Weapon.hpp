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
  
    // Fin de reload
    if (m_isReloading && m_reloadTimer.GetElapsedTime() >= m_reloadCooldown)
    {
        m_isReloading = false;
        m_ammo = m_maxAmmo;
    }

    // Cooldown tir
    if (m_shotTimer.GetElapsedTime() >= m_shotCooldown)
    {
        m_isShooting = false;
    }

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
    if (m_ammo == m_maxAmmo)
    {
        std::cout << "Chargeur plein" << std::endl;
        return;
    }
    m_isReloading = true;
    m_reloadTimer.Start();
    std::cout << "Reload" << std::endl;

}

virtual void TryShoot()
{
    if (!m_isShooting) return;     // on n’a pas demandé à tirer
    if (m_isReloading) return;     // en reload
    if (m_ammo <= 0) return;       // pas de munitions
    if (m_shotTimer.GetElapsedTime() < m_shotCooldown)
        return;                    // cooldown pas fini

    Shoot();                       // tir réel (appel override Rifle)
}


END_SCRIPT

#endif // !WEAPON_HPP_INCLUDED
