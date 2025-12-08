#ifndef KAMIKAZE_HPP_INCLUDED
#define KAMIKAZE_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Enemy.hpp"

using namespace gce;

DECLARE_CHILD_SCRIPT(Kamikaze, Enemy, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::CollisionEnter)

void Start() override
{
    m_HealthMax = new Health<float>(100.f);
}

void Update() override
{

}

void Shoot() override
{

}

void Init(D12PipelineObject* pso) override
{
    MeshRenderer& mesh = *m_pOwner->AddComponent<MeshRenderer>();
    mesh.pGeometry = SHAPES.CUBE;
    mesh.pPso = pso;
    m_pOwner->transform.SetWorldPosition({ 5.f,0.5f,0.f });
    m_pOwner->transform.SetWorldScale({ 1.f,1.f,1.f });
}

void CollisionEnter(GameObject* other) override
{
    
}


END_SCRIPT

#endif