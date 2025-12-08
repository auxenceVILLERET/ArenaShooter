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


void Start() override
{

}


void Update() override
{
    m_DeltaTime = GameManager::DeltaTime();

    Move();
    UpdateDistance();

}

void CollisionEnter(GameObject* other) override
{
    m_pOwner->Destroy();
}

virtual void Init(Vector3f32 dir,Vector3f32 pos, float32 speed, D12PipelineObject* pso)
{
   
}

void Move()
{
    Vector3f32 offset = m_Direction * m_Speed * m_DeltaTime;
    m_pOwner->transform.LocalTranslate(offset);
}

void UpdateDistance()
{
    m_CurrentDistance += m_Speed * m_DeltaTime;

    if (m_CurrentDistance >= m_MaxDistance)
        m_pOwner->Destroy();
}

END_SCRIPT

#endif