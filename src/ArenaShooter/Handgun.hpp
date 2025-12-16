#ifndef HANDGUN_HPP_INCLUDED
#define HANDGUN_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Chrono.h"
#include "Weapon.hpp"
#include "BulletHandgun.hpp"

using namespace gce;

DECLARE_CHILD_SCRIPT(Handgun, Weapon, ScriptFlag::Start | ScriptFlag::Update)

BulletHandgun* m_chargingBullet = nullptr;
float32 m_chargeValue = 0.f;
float32 m_maxCharge = 3.f; 
float32 m_speed = 20.f;
float32 maxSpeed = 20.f;
float32 minSpeed = 8.f;
float32 m_scale;

void Start() override
{
    Weapon::Start();
    m_shotCooldown = 0.3f;
    m_reloadCooldown = 1.5f;

    for (int i = 0; i < 10; i++)
    {
        GameObject& bullet = GameObject::Create(m_pOwner->GetScene());

        MeshRenderer& meshProjectile = *bullet.AddComponent<MeshRenderer>();
        meshProjectile.pGeometry = SHAPES.SPHERE;
        bullet.transform.SetWorldPosition({ 0.0f, 0.0f, 0.0f });
        bullet.transform.SetWorldScale({ 0.2f, 0.2f, 0.2f });
        bullet.SetName("Handgun bullet");

        bullet.AddComponent<SphereCollider>();
        bullet.AddComponent<PhysicComponent>()->SetGravityScale(0.0f);

        m_pProjectiles.PushBack(bullet.AddScript<BulletHandgun>());
    }
}

void Update() override
{
    Weapon::Update();

   
}

void BeginShot() override
{
    if (m_isShooting) return;
    if (m_isReloading) return;
    m_isShooting = true;
    m_chargeValue = 0.f;

    Projectile* proj = GetFirstAvailableProjectile();
    m_chargingBullet = dynamic_cast<BulletHandgun*>(proj);
    m_chargingBullet->SetInactiveColider();

    Vector3f32 pos = m_pOwner->transform.GetWorldPosition() + m_pOwner->transform.GetWorldForward() * 0.5f;

    m_chargingBullet->Activate(pos);
    m_chargingBullet->SetSpeedBullet(0);
}

bool Shoot() override
{
    Weapon::Shoot();

    if (m_isShooting && m_chargingBullet != nullptr)
    {
        float dt = GameManager::DeltaTime();

        m_chargeValue += dt;
        if (m_chargeValue > m_maxCharge)
        {
            m_chargeValue = m_maxCharge;
        }
    
        m_scale = 0.2f + m_chargeValue * 0.5f;
        m_chargingBullet->SetScale(m_scale);


        Vector3f32 pos = m_pOwner->transform.GetWorldPosition() + m_pOwner->transform.GetWorldForward() * m_scale;

        m_chargingBullet->SetPosition(pos);
    }
    return true;
}

void EndShot() override
{
    m_chargingBullet->SetDmgBullet(5.f);
    float32 power = m_chargeValue / m_maxCharge;
    float32 dmg = m_chargingBullet->GetDmgBullet();
    float32 tempDmg = dmg + power * 40.f;
    m_chargingBullet->SetDmgBullet(tempDmg);

    m_speed = maxSpeed - power * (maxSpeed - minSpeed);
    m_chargingBullet->SetSpeedBullet(m_speed);


    if (m_chargingBullet)
    {
        m_chargingBullet->Init(m_pOwner->transform.GetWorldForward(), m_pOwner->transform.GetWorldPosition() + m_pOwner->transform.GetWorldForward() * m_scale, m_speed);
    }

    m_chargingBullet = nullptr;
    m_chargeValue = 0.f;
    m_scale = 0.f;
    m_isShooting = false;
    m_shotTimer.Pause();
    m_shotTimer.Reset();
}

END_SCRIPT

#endif