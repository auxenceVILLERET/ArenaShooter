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
    Weapon::Start();
    m_shotCooldown = 0.3f;
    m_reloadCooldown = 1.5f;

    for (int i = 0; i < 50; i++)
    {
        GameObject& bullet = GameObject::Create(m_pOwner->GetScene());
        MeshRenderer& meshProjectile = *bullet.AddComponent<MeshRenderer>();
        meshProjectile.pGeometry = SHAPES.SPHERE;
        bullet.transform.SetWorldPosition({ 0.0f, 0.0f, 0.0f });
        bullet.transform.SetWorldScale({ 0.3f,0.3f,0.3f });
        bullet.SetName("Riffle bullet");

        bullet.AddComponent<SphereCollider>();
        bullet.AddComponent<PhysicComponent>()->SetGravityScale(0.0f);
        m_pProjectiles.PushBack(bullet.AddScript<BulletRifle>());
    }
}

bool Shoot() override
{
    if (!Weapon::Shoot())
        return false;
    Projectile* proj = GetFirstAvailableProjectile();
    BulletRifle* bulletRifle = dynamic_cast<BulletRifle*>(proj);

    if (bulletRifle)
        bulletRifle->Init(m_pOwner->transform.GetWorldForward(),m_pOwner->transform.GetWorldPosition(), 20.f);

}

END_SCRIPT

#endif // !RIFLE_HPP_INCLUDED