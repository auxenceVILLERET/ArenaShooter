#ifndef BULLETTANK_HPP_INCLUDED
#define BULLETTANK_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Shapes.h"
#include "Chrono.h"
#include "Projectile.hpp"

using namespace gce;

DECLARE_CHILD_SCRIPT(BulletTank, Projectile, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::CollisionEnter)

void Start() override
{
    m_MaxDistance = 40.f;
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
    if (other->GetName() == "Tank")
        return;

    m_pOwner->SetActive(false);
}

END_SCRIPT

#endif