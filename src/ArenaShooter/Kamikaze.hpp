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
    m_Hp = new Health<float>(100.f);
    MeshRenderer& mesh = *m_pOwner->AddComponent<MeshRenderer>();
    mesh.pGeometry = SHAPES.CUBE;
    m_pOwner->transform.SetWorldPosition({ 5.f,0.5f,0.f });
    m_pOwner->transform.SetWorldScale({ 1.f,1.f,1.f });
}

void Update() override
{
    Enemy::Update();
}

void Shoot() override
{

}

END_SCRIPT

#endif