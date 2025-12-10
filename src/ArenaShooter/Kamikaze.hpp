#ifndef KAMIKAZE_HPP_INCLUDED
#define KAMIKAZE_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Enemy.hpp"

using namespace gce;

DECLARE_CHILD_SCRIPT(Kamikaze, Enemy, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::CollisionEnter)

void Awake() override
{
    m_Hp = new Health<float>(100.f);
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