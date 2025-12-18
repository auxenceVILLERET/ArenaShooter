#ifndef BULLETSHOTGUN_HPP_INCLUDED
#define BULLETSHOTGUN_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Shapes.h"
#include "Chrono.h"
#include "Projectile.hpp"

using namespace gce;

DECLARE_CHILD_SCRIPT(BulletShotgun, Projectile, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::CollisionEnter)

void Start()
{
    m_MaxDistance = 15.f;
    m_dmgBullet = 6.f;

    m_pOwner->SetActive(false);
}

void Update()
{
    Projectile::Update();
}


void Init(Vector3f32 dir, Vector3f32 pos, float32 speed)
{
    Projectile::Init(dir, pos, speed);
    OrientFace(dir);
}

void OrientFace(Vector3f32 dir)
{
    float yaw = atan2(dir.x, dir.z);
    float pitch = asin(-dir.y);
    Quaternion q = Quaternion::RotationEuler(pitch, yaw, 0);
    m_pOwner->transform.SetLocalRotation(q);
}

END_SCRIPT

#endif // BULLETSHOTGUN_HPP_INCLUDED
