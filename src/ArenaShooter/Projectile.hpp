#ifndef PROJECTILE_HPP_INCLUDED
#define PROJECTILE_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Shapes.h"
#include "Chrono.h"
using namespace gce;

DECLARE_SCRIPT(Projectile, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::CollisionEnter)

Vector3f32 m_Direction;
Vector3f32 m_Position;
float32 m_Speed;
float32 m_MaxDistance;
float32 m_CurrentDistance = 0.f;
float32 m_DeltaTime;
float32 m_dmgBullet;

void Start()
{

}

void Update()
{
    m_DeltaTime = GameManager::DeltaTime();

    Move();
    UpdateDistance();

}

void CollisionEnter(GameObject* other)
{
    if (m_pOwner->GetName() == other->GetName())
        return;
    
    return m_pOwner->SetActive(false);
}

virtual void Init(Vector3f32 dir,Vector3f32 pos, float32 speed)
{
    m_pOwner->SetActive(true);
    
    m_Direction = dir;
    m_Position = pos;
    m_Speed = speed;
    m_CurrentDistance = 0.f;

    m_pOwner->transform.SetWorldPosition(m_Position);
}

bool IsActive() { return m_pOwner->IsActive(); }

void Move()
{
    Vector3f32 offset = m_Direction * m_Speed * m_DeltaTime;
    m_pOwner->transform.LocalTranslate(offset);
}

virtual void UpdateDistance()
{
    m_CurrentDistance += m_Speed * m_DeltaTime;

    if (m_CurrentDistance >= m_MaxDistance)
        m_pOwner->SetActive(false);
}

float32 GetDmgBullet()
{
    return m_dmgBullet;
}

void SetDmgBullet(float32 newDmg)
{
    m_dmgBullet = newDmg;
}

float32 GetSpeedBullet()
{
    return m_Speed;
}

void SetSpeedBullet(float32 newSpeed)
{
    m_Speed = newSpeed;
}
END_SCRIPT

#endif