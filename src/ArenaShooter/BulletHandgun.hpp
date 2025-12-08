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

}

void Update() override
{
    Projectile::Update();
}

void Init(Vector3f32 dir, Vector3f32 pos, float32 speed, D12PipelineObject* pso) override
{
    m_Direction = dir;
    m_Position = pos;
    m_Speed = speed;
    m_MaxDistance = 15.f; // Les projectiles de shotgun ont une portée plus courte

    MeshRenderer& meshProjectile = *m_pOwner->AddComponent<MeshRenderer>();
    meshProjectile.pGeometry = SHAPES.SPHERE; // On peut choisir une autre forme pour les projectiles
    meshProjectile.pPso = pso;
    m_pOwner->transform.SetWorldPosition(m_Position);
    m_pOwner->transform.SetWorldScale({ 0.2f, 0.2f, 0.2f }); // Taille des projectiles
}

END_SCRIPT

#endif