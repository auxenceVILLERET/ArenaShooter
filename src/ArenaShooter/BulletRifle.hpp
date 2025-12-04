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

}

void Update() override
{

}

void Init(Vector3f32 dir, float32 speed, D12PipelineObject* pso) override
{
    m_Direction = dir;
    m_Speed = speed;
    m_MaxDistance = 10.f;

    MeshRenderer& meshProjectile = *m_pOwner->AddComponent<MeshRenderer>();
    meshProjectile.pGeometry = SHAPES.CUBE;
    meshProjectile.pPso = pso;

}


END_SCRIPT

#endif