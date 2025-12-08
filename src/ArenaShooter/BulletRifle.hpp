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
    m_dmgBullet = 10.f;
}

void Update() override
{
    Projectile::Update();
}

void Init(Vector3f32 dir,Vector3f32 pos, float32 speed, D12PipelineObject* pso) override
{
    m_Direction = dir;
    m_Position = pos;
    m_Speed = speed;
    m_MaxDistance = 30.f;


    m_pOwner->AddComponent<SphereCollider>();
    m_pOwner->AddComponent<PhysicComponent>()->SetGravityScale(0.0f);

    MeshRenderer& meshProjectile = *m_pOwner->AddComponent<MeshRenderer>();
    meshProjectile.pGeometry = SHAPES.SPHERE;
    meshProjectile.pPso = pso;
    m_pOwner->transform.SetWorldPosition(m_Position);
    m_pOwner->transform.SetWorldScale({ 0.3f,0.3f,0.3f });

}


END_SCRIPT

#endif