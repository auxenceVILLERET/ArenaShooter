#ifndef WEAPONCONTROLLER_HPP_INCLUDED
#define WEAPONCONTROLLER_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "Weapon.hpp"

using namespace gce;

DECLARE_SCRIPT(WeaponController, ScriptFlag::Start)

private:
    std::vector<Weapon*> m_weapons;   
    int m_currentIndex = -1;          
public:
    void AddWeapon(Weapon* weapon)
    {
        m_weapons.push_back(weapon);

        if (m_currentIndex == -1)
        {
            m_currentIndex = 0;
            weapon->GetOwner()->SetActive(true);
        }
        else
            weapon->GetOwner()->SetActive(false);
    }

    Weapon* GetCurrentWeapon()
    {
        if (m_currentIndex < 0 || m_currentIndex >= m_weapons.size())
            return nullptr;
        return m_weapons[m_currentIndex];
    }

    void EquipWeapon(int index)
    {
        if (index < 0 || index >= m_weapons.size())
            return;

        if (m_currentIndex != -1)
            m_weapons[m_currentIndex]->GetOwner()->SetActive(false);

        m_currentIndex = index;
        m_weapons[m_currentIndex]->GetOwner()->SetActive(true);
    }

    void NextWeapon()
    {
        if (m_weapons.size() <= 1)
            return;

        int next = (m_currentIndex + 1) % m_weapons.size();
        EquipWeapon(next);
    }

    void Shoot()
    {
        Weapon* w = GetCurrentWeapon();
        if (!w) return;
        w->Shoot();
    }


    void BeginShot()
    {
        Weapon* w = GetCurrentWeapon();
        if (!w) return;
        w->BeginShot();
    }

    void EndShot()
    {
        Weapon* w = GetCurrentWeapon();
        if (!w) return;
        w->EndShot();
    }

    void Reload()
    {
        Weapon* w = GetCurrentWeapon();
        if (!w) return;
        w->Reload();
    }

    END_SCRIPT

#endif
