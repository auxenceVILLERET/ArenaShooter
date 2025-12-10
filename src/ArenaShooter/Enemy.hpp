#ifndef ENEMY_HPP_INCLUDED
#define ENEMY_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Health.h"
#include "Projectile.hpp"
#include "StateMachine/StateMachine.h"

using namespace gce;

bool IsPlayerClose(GameObject* me)
{
	GameObject* player = me->GetScript<Enemy>()->m_pPlayer;
	if (player == nullptr) return false;

	Vector3f32 DistVect = player->transform.GetLocalPosition() - me->transform.GetLocalPosition();
	float distance = DistVect.Norm();

	Console::Log("Distance entre joueur et ennemi : " + std::to_string(distance));

	return distance < 8.0f; // Seuil de distance
}

bool IsPlayerVeryClose(GameObject* me)
{
	GameObject* player = me->GetScript<Enemy>()->m_pPlayer;
	if (player == nullptr) return false;

	Vector3f32 DistVect = player->transform.GetLocalPosition() - me->transform.GetLocalPosition();
	float distance = DistVect.Norm();

	Console::Log("Distance entre joueur et ennemi : " + std::to_string(distance));

	return distance < 2.0f; // Seuil de distance
}

bool IsPlayerFar(GameObject* me)
{
	GameObject* player = me->GetScript<Enemy>()->m_pPlayer;
	if (player == nullptr) return false;

	Vector3f32 DistVect = player->transform.GetLocalPosition() - me->transform.GetLocalPosition();
	float distance = DistVect.Norm();

	Console::Log("Distance entre joueur et ennemi : " + std::to_string(distance));

	return distance < 12.0f; // Seuil de distance
}

void OnBeginIdle(GameObject* me)
{

}

void OnUpdateIdle(GameObject* me)
{

}

void OnEndIdle(GameObject* me)
{

}

void OnBeginAttack(GameObject* me)
{
	
}

void OnUpdateAttack(GameObject* me)
{

}

void OnEndAttack(GameObject* me)
{

}

void OnBeginChase(GameObject* me)
{

}

void OnUpdateChase(GameObject* me)
{

}

void OnEndChase(GameObject* me)
{

}




DECLARE_SCRIPT(Enemy, ScriptFlag::Awake | ScriptFlag::Update | ScriptFlag::CollisionEnter)

Health<float>* m_Hp;
StateMachine& m_sm = StateMachine::Create(*m_pOwner);
GameObject* m_pPlayer;

void Awake() override
{

	String idle = "Idle";
	m_sm.AddAction(idle, OnBeginIdle, OnUpdateIdle, OnEndIdle);

	String chase = "Chase";
	m_sm.AddAction(chase, OnBeginChase, OnUpdateChase, OnEndChase);

	String attack = "Attack";
	m_sm.AddAction(attack, OnBeginAttack, OnUpdateAttack, OnEndAttack);


	StateMachine::Condition closePlayerCondition = { IsPlayerClose };
	Vector<StateMachine::Condition> closePlayerConditions;
	closePlayerConditions.PushBack(closePlayerCondition);
	m_sm.AddTransition(closePlayerConditions, chase);

	StateMachine::Condition veryClosePlayerCondition = { IsPlayerVeryClose };
	Vector<StateMachine::Condition> veryClosePlayerConditions;
	closePlayerConditions.PushBack(veryClosePlayerCondition);
	m_sm.AddTransition(veryClosePlayerConditions, attack);

	StateMachine::Condition farPlayerCondition = { IsPlayerFar };
	Vector<StateMachine::Condition> farPlayerConditions;
	closePlayerConditions.PushBack(farPlayerCondition);
	m_sm.AddTransition(farPlayerConditions, idle);

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
	if (pOther->GetScript<BulletShotgun>())
	{
		m_Hp->TakeDamage(pOther->GetScript<BulletShotgun>()->GetDmgBullet());
		std::cout << m_Hp->GetHealth() << std::endl;
	}
}

void SetPlayer(GameObject* player)
{
	m_pPlayer = player;
}

END_SCRIPT
#endif
