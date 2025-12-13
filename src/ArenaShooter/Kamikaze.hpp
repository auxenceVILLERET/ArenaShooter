#ifndef KAMIKAZE_HPP_INCLUDED
#define KAMIKAZE_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Enemy.hpp"
#include "GameManager.h"
#include "StateMachine/StateMachine.h"

using namespace gce;

DECLARE_CHILD_SCRIPT(Kamikaze, Enemy, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::CollisionEnter)

StateMachine* m_pSm = nullptr;

void Awake() override
{
	m_speed = 2.50f;
	Enemy::Awake();
	m_Hp = new Health<float>(100.f);
	
	m_pSm = GameManager::GetStateSystem().CreateStateMachine(m_pOwner);
	String idle = "Idle";
	m_pSm->AddAction(idle, [this]() {this->OnBeginIdle(); }, [this]() {this->OnUpdateIdle(); }, [this]() {this->OnEndIdle(); });
 
	String chase = "Chase";
	m_pSm->AddAction(chase, [this]() {this->OnBeginChase(); }, [this]() {this->OnUpdateChase(); }, [this]() {this->OnEndChase(); });
 
	String attack = "Attack";
	m_pSm->AddAction(attack, [this]() {this->OnBeginAttack(); }, [this]() {this->OnUpdateAttack(); }, [this]() {this->OnEndAttack(); });
 
 
	StateMachine::Condition closePlayerCondition = { [this]() { return this->IsPlayerClose(); } };
	Vector<StateMachine::Condition> closePlayerConditions;
	closePlayerConditions.PushBack(closePlayerCondition);
	m_pSm->AddTransition(closePlayerConditions, idle, chase);
 
	StateMachine::Condition veryClosePlayerCondition = { [this]() { return this->IsPlayerVeryClose(); } };
	Vector<StateMachine::Condition> veryClosePlayerConditions;
	veryClosePlayerConditions.PushBack(veryClosePlayerCondition);
	// m_pSm->AddTransition(veryClosePlayerConditions, chase, attack);
 
	StateMachine::Condition farPlayerCondition = { [this]() { return this->IsPlayerFar(); } };
	Vector<StateMachine::Condition> farPlayerConditions;
	farPlayerConditions.PushBack(farPlayerCondition);
	m_pSm->AddTransition(farPlayerConditions,chase, idle);
}

void Update() override
{
	if (m_Hp->GetHealth() <= 0.f)
	{
		m_Hp->SetIsAlive(false);
		GameManager::GetStateSystem().DestroyStateMachine(m_pOwner);
		m_pOwner->Destroy();
	}
	
	Enemy::Update();
}

void Shoot() override
{

}

bool IsPlayerClose()
{
	GameObject* player = m_pOwner->GetScript<Kamikaze>()->m_pPlayer;
	if (player == nullptr) return false;
	Vector3f32 DistVect = player->transform.GetLocalPosition() - m_pOwner->transform.GetLocalPosition();
	float distance = DistVect.Norm();
	return distance < 20.0f; // Seuil de distance
}

bool IsPlayerVeryClose()
{
	GameObject* player = m_pOwner->GetScript<Kamikaze>()->m_pPlayer;
	if (player == nullptr) return false;
	Vector3f32 DistVect = player->transform.GetLocalPosition() - m_pOwner->transform.GetLocalPosition();
	float distance = DistVect.Norm();
	return distance < 8.0f; // Seuil de distance
}

bool IsPlayerFar()
{
	GameObject* player = m_pOwner->GetScript<Kamikaze>()->m_pPlayer;
	if (player == nullptr) return false; Vector3f32 DistVect = player->transform.GetLocalPosition() - m_pOwner->transform.GetLocalPosition();
	float distance = DistVect.Norm();
	return distance > 25.0f; // Seuil de distance
}

void OnBeginIdle() { Console::Log("Idle"); }
void OnUpdateIdle() {}
void OnEndIdle() {}

void OnBeginChase()
{
	Console::Log("Chase");
}
void OnUpdateChase()
{
	SetPath(m_pPlayer->transform.GetWorldPosition());
}
void OnEndChase() {}

void OnBeginAttack() { Console::Log("Attack"); }
void OnUpdateAttack() {}
void OnEndAttack() {}

END_SCRIPT





#endif