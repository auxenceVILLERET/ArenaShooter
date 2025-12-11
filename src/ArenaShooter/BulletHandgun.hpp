#ifndef BULLETHANDGUN_HPP_INCLUDED
#define BULLETHANDGUN_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Shapes.h"
#include "Chrono.h"
#include "Projectile.hpp"

using namespace gce;

DECLARE_CHILD_SCRIPT(BulletHandgun, Projectile, ScriptFlag::Start | ScriptFlag::Update)

void Start() override
{
    m_MaxDistance = 10.f;
    m_dmgBullet = 10.f;

    m_pOwner->SetActive(false);
}

void Update() override
{
    Projectile::Update();
}

void Init(Vector3f32 dir, Vector3f32 pos, float32 speed) override
{
    Projectile::Init(dir, pos, speed);
    m_pOwner->GetComponent<SphereCollider>()->SetActive(true);

}

void Activate(Vector3f32 pos)
{
    m_pOwner->SetActive(true);
    m_pOwner->transform.SetWorldPosition(pos);
    m_CurrentDistance = 0.f;
}

void SetScale(float s)
{
    m_pOwner->transform.SetWorldScale({ s, s, s });
}

void SetPosition(Vector3f32 pos)
{
    m_pOwner->transform.SetWorldPosition(pos);
}

void SetInactiveColider()
{
    m_pOwner->GetComponent<SphereCollider>()->SetActive(false);
}
END_SCRIPT

#endif