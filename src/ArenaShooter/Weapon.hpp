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

}

virtual void BeginShot()
{
	if (m_isReloading == true) return;
	if (m_isShooting == true) return;

	m_isShooting = true;
}

template<typename T>

void Shoot() 
{
	

}

virtual void EndShot()
{

}

void Reload()
{
	m_ammo = m_maxAmmo;
}

END_SCRIPT

#endif // ! WEAPON_HPP_DEFINED
