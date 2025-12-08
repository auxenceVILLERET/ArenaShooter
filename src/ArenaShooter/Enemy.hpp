#ifndef ENEMY_HPP_INCLUDED
#define ENEMY_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Health.h"
#include "Projectile.hpp"

using namespace gce;

DECLARE_SCRIPT(Enemy, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::CollisionEnter)

Health<float>* m_HealthMax;

void Start() override
{

}

void Update() override
{

}

virtual void Shoot()
{

}


virtual void Init(D12PipelineObject* pso)
{

}

void CollisionEnter(GameObject* pOther) override
{
	if(pOther->GetScript<Projectile>())
	{
		m_HealthMax->TakeDamage(pOther->GetScript<Projectile>()->GetDmgBullet());
		std::cout << m_HealthMax << std::endl;
	}
}

END_SCRIPT
#endif
