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

    Geometry* pGeo = GeometryFactory::LoadGeometry(RES_PATH"res/ArenaShooter/Obj/laser.obj");
    Texture* albedo = new Texture(RES_PATH"res/ArenaShooter/Obj/LaserTxtBlue.png");

    for (int i = 0; i < 50; i++)
    {
        GameObject& bullet = m_pCustomScene->AddObject();
        MeshRenderer& meshProjectile = *bullet.AddComponent<MeshRenderer>();
        meshProjectile.pGeometry = pGeo;
        meshProjectile.pMaterial->albedoTextureID = albedo->GetTextureID();
        meshProjectile.pMaterial->useTextureAlbedo = 1.f;
        bullet.transform.SetWorldPosition({ 0.0f, 0.0f, 0.0f });
        bullet.transform.SetWorldScale({ 1.3f,1.3f,1.3f });
        bullet.SetName("Rifle bullet");

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
        bulletRifle->Init(m_pOwner->transform.GetWorldForward(), m_pOwner->transform.GetWorldPosition() + m_pOwner->transform.GetWorldForward() * 1.2f, 40.f);

}

END_SCRIPT

#endif // !RIFLE_HPP_INCLUDED