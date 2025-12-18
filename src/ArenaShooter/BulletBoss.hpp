#ifndef BULLETBOSS_HPP_INCLUDED
#define BULLETBOSS_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Shapes.h"
#include "Chrono.h"
#include "Projectile.hpp"

using namespace gce;

DECLARE_CHILD_SCRIPT(BulletBoss, Projectile, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::CollisionEnter)

void Start() override
{
    m_MaxDistance = 100.f;
    m_dmgBullet = 1.f;

    m_pOwner->SetActive(false);
}

void Update() override
{
    Projectile::Update();
}

void Init(Vector3f32 dir, Vector3f32 pos, float32 speed) override
{
    Projectile::Init(dir, pos, speed);
}

void CollisionEnter(GameObject* other) override
{
    if (other->GetName() == "Boss" || other->GetName() == "Boss bullet" || other->GetName() == "Riffle bullet" || other->GetName() == "Shotgun bullet" || other->GetName() == "Handgun bullet" || other->GetName() == "Kamikaze" || other->GetName() == "Energy Orb")
        return;

    m_pOwner->SetActive(false);
}

END_SCRIPT

#endif