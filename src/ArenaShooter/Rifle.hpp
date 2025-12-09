#ifndef RIFLE_HPP_INCLUDED
#define RIFLE_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Chrono.h"
#include "Weapon.hpp"
#include "BulletRifle.hpp"

using namespace gce;

DECLARE_CHILD_SCRIPT(Rifle, Weapon, ScriptFlag::Start | ScriptFlag::Update)

void Start() override
{
    m_shotCooldown = 0.3f;
    m_reloadCooldown = 1.5f;
    MeshRenderer& meshProjectileRifle = *m_pOwner->AddComponent<MeshRenderer>();
    meshProjectileRifle.pGeometry = SHAPES.SPHERE;

    for (int i = 0; i < 50; i++)
    {
        GameObject& bullet = GameObject::Create(m_pOwner->GetScene());
        m_pProjectiles.PushBack(bullet.AddScript<BulletRifle>());
    }
}

void Shoot() override
{
    m_shotTimer.Reset();
    m_shotTimer.Start();
    m_heat += m_heatPerShot;

    Projectile* proj = GetFirstAvailableProjectile();
    BulletRifle* bulletRifle = dynamic_cast<BulletRifle*>(proj);

    if (bulletRifle)
        bulletRifle->Init(m_pOwner->transform.GetWorldForward(),m_pOwner->transform.GetWorldPosition(), 20.f);

}

END_SCRIPT

#endif // !RIFLE_HPP_INCLUDED