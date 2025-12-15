#ifndef BAZOOKA_HPP_INCLUDED
#define BAZOOKA_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "Weapon.hpp"
#include "WeaponController.hpp"

using namespace gce;

DECLARE_CHILD_SCRIPT(Bazooka, Weapon, ScriptFlag::Awake | ScriptFlag::Update)

int m_weaponIndex = 3;

void Awake() override
{
	Weapon::Awake();
}

void Update() override
{
	Weapon::Update();
}

bool Shoot() override
{
	if(m_isShooting)
		return false;

	Console::Log("Bazooka Shoot!");

    LockMyself();

	return true;
}

void LockMyself()
{
	if (m_pWeaponController == nullptr)
		return;

	m_pWeaponController->LockWeapon(m_weaponIndex);
	m_pWeaponController->EquipFirstUnlocked();
}

END_SCRIPT
#endif // !BAZOOKA_HPP_INCLUDED
