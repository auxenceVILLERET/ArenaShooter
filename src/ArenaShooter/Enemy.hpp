#ifndef ENEMY_HPP_INCLUDED
#define ENEMY_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Health.h"
#include "Projectile.hpp"
#include "StateMachine/StateMachine.h"

using namespace gce;

DECLARE_SCRIPT(Enemy, ScriptFlag::Awake | ScriptFlag::Update | ScriptFlag::CollisionEnter)

Health<float>* m_Hp;
GameObject* m_pPlayer;

void Awake() override
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

void SetPlayer(GameObject* player)
{
	m_pPlayer = player;
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
	if (pOther->GetScript<BulletShotgun>())
	{
		m_Hp->TakeDamage(pOther->GetScript<BulletShotgun>()->GetDmgBullet());
		std::cout << m_Hp->GetHealth() << std::endl;
	}
}

END_SCRIPT


#endif
