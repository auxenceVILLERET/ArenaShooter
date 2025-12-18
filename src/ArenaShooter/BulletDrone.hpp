#ifndef BULLETDRONE_HPP_INCLUDED
#define BULLETDRONE_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Shapes.h"
#include "Chrono.h"
#include "Projectile.hpp"

using namespace gce;

DECLARE_CHILD_SCRIPT(BulletDrone, Projectile, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::CollisionEnter)

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
	OrientFace(dir);
}

void CollisionEnter(GameObject* other) override
{
    String otherName = other->GetName();
    if (otherName == "Drone" || otherName == "Rifle bullet" || otherName == "Handgun bullet" || otherName == "Shotgun bullet" || otherName == "Bazooka bullet" || otherName == "Drone bullet" || otherName == "Tank bullet")
        return;

    m_pOwner->SetActive(false);
}

void OrientFace(Vector3f32 dir)
{
    float yaw = atan2(dir.x, dir.z);
    float pitch = asin(-dir.y);
    Quaternion q = Quaternion::RotationEuler(pitch, yaw, 0);
    m_pOwner->transform.SetLocalRotation(q);
}


END_SCRIPT

#endif