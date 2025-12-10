#ifndef ENEMY_HPP_INCLUDED
#define ENEMY_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Health.h"
#include "Projectile.hpp"

using namespace gce;

DECLARE_SCRIPT(Enemy, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::CollisionEnter)

Health<float>* m_Hp;

void Start() override
{

}

void Update() override
{
	if (m_Hp->GetHealth() <= 0.f)
	{
		m_Hp->SetIsAlive(false);
	}

	if (m_Hp->GetIsAlive() == true) return;

	m_pOwner->Destroy();

}

virtual void Shoot()
{

}

void CollisionEnter(GameObject* pOther) override
{
	if(pOther->GetScript<BulletRifle>())
	{
		m_Hp->TakeDamage(pOther->GetScript<BulletRifle>()->GetDmgBullet());
		std::cout << m_Hp->GetHealth() << std::endl;
	}
}

END_SCRIPT
#endif
