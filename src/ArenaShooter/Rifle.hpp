#ifndef RIFLE_HPP_INCLUDED
#define RIFLE_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Chrono.h"
#include "Weapon.hpp"
#include "BulletRifle.hpp"

using namespace gce;

DECLARE_CHILD_SCRIPT(Rifle, Weapon, ScriptFlag::Start | ScriptFlag::Update)

void Start() override
{
    m_maxAmmo = 25;
    m_ammo = 25;
    m_shotCooldown = 0.1f;
    m_reloadCooldown = 1.5f;

    m_pOwner->transform.SetWorldPosition({ 0,2,0 });
    m_pOwner->transform.SetWorldScale({ 1.f ,1.f,1.f });

}

void Shoot() override
{
    m_ammo -= 1;
    m_shotTimer.Start();

    GameObject& bullet = GameObject::Create(m_pOwner->GetScene());
    bullet.AddScript<BulletRifle>()->Init(m_pOwner->transform.GetWorldForward(), 2.f, m_PSO);

}

void Init(D12PipelineObject* pso) override
{
    m_PSO = pso;

    MeshRenderer& meshProjectileRifle = *m_pOwner->AddComponent<MeshRenderer>();
    meshProjectileRifle.pGeometry = SHAPES.CUBE;
    meshProjectileRifle.pPso = pso;
}

END_SCRIPT

#endif // !RIFLE_HPP_INCLUDED