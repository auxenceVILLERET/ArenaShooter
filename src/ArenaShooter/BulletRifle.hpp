#ifndef BULLETRIFLE_HPP_INCLUDED
#define BULLETRIFLE_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Shapes.h"
#include "Chrono.h"
#include "Projectile.hpp"

using namespace gce;

DECLARE_CHILD_SCRIPT(BulletRifle, Projectile, ScriptFlag::Start | ScriptFlag::Update)

void Start() override
{
    MeshRenderer& meshProjectile = *m_pOwner->AddComponent<MeshRenderer>();
    meshProjectile.pGeometry = SHAPES.SPHERE;
    m_pOwner->transform.SetWorldPosition({0.0f, 0.0f, 0.0f});
    m_pOwner->transform.SetWorldScale({ 0.3f,0.3f,0.3f });
    m_pOwner->SetName("Riffle bullet");

    m_pOwner->AddComponent<SphereCollider>();
    m_pOwner->AddComponent<PhysicComponent>()->SetGravityScale(0.0f);

    m_MaxDistance = 30.f;
    m_dmgBullet = 10.f;

    m_pOwner->SetActive(false);
}

void Update() override
{
    Projectile::Update();
}

void Init(Vector3f32 dir,Vector3f32 pos, float32 speed) override
{
    Projectile::Init(dir,pos,speed);
}

END_SCRIPT

#endif