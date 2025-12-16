#ifndef WEAPONCONTROLLER_HPP_INCLUDED
#define WEAPONCONTROLLER_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "Weapon.hpp"

using namespace gce;

DECLARE_SCRIPT(WeaponController, ScriptFlag::Start | ScriptFlag::Update)

private:
    std::vector<Weapon*> m_weapons;   
    std::vector<bool> m_weaponUnlocked;

    int m_currentIndex = -1;          
public:

    void Start() override
    {
    }

    void Update() override
    {
        for (int i = 0; i < m_weapons.size(); i++)
        {
            if(i == m_currentIndex)
                m_weapons[i]->GetOwner()->SetActive(true);
            else
				m_weapons[i]->GetOwner()->SetActive(false);
        }
	}

    void AddWeapon(Weapon* weapon, bool unlocked = true)
    {
        m_weapons.push_back(weapon);
        m_weaponUnlocked.push_back(unlocked);

		weapon->SetWeaponController(this);
        weapon->GetOwner()->SetActive(false);

        if (m_currentIndex == -1 && unlocked)
        {
            m_currentIndex = m_weapons.size() - 1;
            weapon->GetOwner()->SetActive(true);
        }
        else
        {
            weapon->GetOwner()->SetActive(false);
        }
    }

    void UnlockWeapon(int index)
    {
        if (index < 0 || index >= m_weaponUnlocked.size())
            return;

        m_weaponUnlocked[index] = true;
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

        if (m_weaponUnlocked[index] == false)
            return Console::Log("Lock");

        if (m_currentIndex != -1)
            m_weapons[m_currentIndex]->GetOwner()->SetActive(false);

        m_currentIndex = index;
        m_weapons[m_currentIndex]->GetOwner()->SetActive(true);
    }

    void LockWeapon(int index)
    {
        if (index < 0 || index >= m_weaponUnlocked.size())
            return;

        m_weaponUnlocked[index] = false;
    }

    void EquipFirstUnlocked()
    {
        for (int i = 0; i < m_weapons.size(); ++i)
        {
            if (m_weaponUnlocked[i])
            {
                EquipWeapon(i);
                return;
            }
        }
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

    END_SCRIPT

#endif
