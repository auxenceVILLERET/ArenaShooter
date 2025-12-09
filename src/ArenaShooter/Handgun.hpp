#ifndef HANDGUN_HPP_INCLUDED
#define HANDGUN_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Chrono.h"
#include "Weapon.hpp"
#include "BulletHandgun.hpp"

using namespace gce;

DECLARE_CHILD_SCRIPT(Handgun, Weapon, ScriptFlag::Start | ScriptFlag::Update)


void Start() override
{
    m_shotCooldown = 0.3f;
    m_reloadCooldown = 1.5f;
}

void Shoot() override
{
    m_shotTimer.Start();
    m_heat += m_heatPerShot;

    GameObject& bullet = GameObject::Create(m_pOwner->GetScene());
    bullet.AddScript<BulletHandgun>()->Init(m_pOwner->transform.GetWorldForward(), m_pOwner->transform.GetWorldPosition(), 20.f, m_PSO);

}

void Init(D12PipelineObject* pso) override
{
    m_PSO = pso;

    MeshRenderer& meshProjectileHandgun = *m_pOwner->AddComponent<MeshRenderer>();
    meshProjectileHandgun.pGeometry = SHAPES.SPHERE;
    meshProjectileHandgun.pPso = pso;
}


END_SCRIPT

#endif