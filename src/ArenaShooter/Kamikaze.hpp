#ifndef KAMIKAZE_HPP_INCLUDED
#define KAMIKAZE_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Enemy.hpp"
#include "GameManager.h"
#include "StateMachine/StateMachine.h"
#include "EnergyOrb.hpp"

using namespace gce;

DECLARE_CHILD_SCRIPT(Kamikaze, Enemy, ScriptFlag::Awake | ScriptFlag::Update | ScriptFlag::CollisionEnter | ScriptFlag::Destroy)

StateMachine* m_pSm = nullptr;

float32 explosionRadius = 2.0f;

bool isBlocked = false;
Chrono blockedChrono;
float32 blockedToggleTime = 3.00f;

Chrono chargeChrono;
float32 chargeTime = 0.5f;

Vector3f32 finalDir;

void Awake() override
{
	Enemy::Awake();
	m_speed = 2.50f;
	m_Hp = new Health<float>(1.f);
	
	m_pSm = GameManager::GetStateSystem().CreateStateMachine(m_pOwner);
	String idle = "Idle";
	m_pSm->AddAction(idle, [this]() {this->OnBeginIdle(); }, [this]() {this->OnUpdateIdle(); }, [this]() {this->OnEndIdle(); });
 
	String chase = "Chase";
	m_pSm->AddAction(chase, [this]() {this->OnBeginChase(); }, [this]() {this->OnUpdateChase(); }, [this]() {this->OnEndChase(); });
 
	String charge = "Charge";
	m_pSm->AddAction(charge, [this]() {this->OnBeginCharge(); }, [this]() {this->OnUpdateCharge(); }, [this]() {this->OnEndCharge(); });

	String dash = "Dash";
	m_pSm->AddAction(dash, [this]() {this->OnBeginDash(); }, [this]() {this->OnUpdateDash(); }, [this]() {this->OnEndDash(); });
 
	StateMachine::Condition closePlayerCondition = { [this]() { return this->IsPlayerClose(); } };
	Vector<StateMachine::Condition> closePlayerConditions;
	closePlayerConditions.PushBack(closePlayerCondition);
	m_pSm->AddTransition(closePlayerConditions, idle, chase);
 
	StateMachine::Condition veryClosePlayerCondition = { [this]() { return this->IsPlayerVeryClose(); } };
	Vector<StateMachine::Condition> veryClosePlayerConditions;
	veryClosePlayerConditions.PushBack(veryClosePlayerCondition);
	m_pSm->AddTransition(veryClosePlayerConditions, chase, charge);
 
	StateMachine::Condition farPlayerCondition = { [this]() { return this->IsPlayerFar(); } };
	Vector<StateMachine::Condition> farPlayerConditions;
	farPlayerConditions.PushBack(farPlayerCondition);
	m_pSm->AddTransition(farPlayerConditions,chase, idle);

	StateMachine::Condition blockedCondition = { [this]() { return this->IsBlocked(); }};
	Vector<StateMachine::Condition> blockedConditions;
	blockedConditions.PushBack(blockedCondition);
	m_pSm->AddTransition(blockedConditions, chase, idle);

	StateMachine::Condition dashCondition = { [this]() { return this->HasCharged(); }};
	Vector<StateMachine::Condition> dashConditions;
	dashConditions.PushBack(dashCondition);
	m_pSm->AddTransition(dashConditions, charge, dash);
}

void Update() override
{
	if (m_Hp->GetHealth() <= 0.f)
	{
		m_Hp->SetIsAlive(false);
		m_pOwner->SetActive(false);

		GameObject& energyOrb = GameObject::Create(m_pOwner->GetScene());
		MeshRenderer& meshEnergyOrb = *energyOrb.AddComponent<MeshRenderer>();
		meshEnergyOrb.pGeometry = SHAPES.SPHERE;
		energyOrb.transform.SetWorldScale({ 0.5f,0.5f,0.5f });
		energyOrb.transform.SetWorldPosition(m_pOwner->transform.GetWorldPosition());
		energyOrb.AddScript<EnergyOrb>();
		energyOrb.AddComponent<BoxCollider>();
		energyOrb.AddComponent<PhysicComponent>()->SetMass(1.0f);
	}
	
	Enemy::Update();
}

void Destroy() override
{
	GameManager::GetStateSystem().DestroyStateMachine(m_pOwner);
}

void Explode()
{
	if (m_pPlayer != nullptr)
	{
		float32 distanceToPlayer = (m_pPlayer->transform.GetLocalPosition() - m_pOwner->transform.GetLocalPosition()).Norm();
		if (distanceToPlayer < explosionRadius)
			m_pPlayer->GetScript<Player>()->TakeDamage(1);
	}
	
	m_pOwner->SetActive(false);
}

void Shoot() override
{

}

void CollisionEnter(GameObject* other) override
{
	Enemy::CollisionEnter(other);
	if (m_pSm->actualAction == "Dash")
	{
		Explode();
	}
}

bool IsPlayerClose()
{
	GameObject* player = m_pPlayer;
	if (player == nullptr) return false;
	Vector3f32 DistVect = player->transform.GetLocalPosition() - m_pOwner->transform.GetLocalPosition();
	float distance = DistVect.Norm();
	return distance < 20.0f; // Seuil de distance
}

bool IsPlayerVeryClose()
{
	GameObject* player = m_pPlayer;
	if (player == nullptr) return false;
	Vector3f32 DistVect = player->transform.GetLocalPosition() - m_pOwner->transform.GetLocalPosition();
	float distance = DistVect.Norm();
	return distance < 10.0f; // Seuil de distance
}

bool IsPlayerFar()
{
	GameObject* player = m_pPlayer;
	if (player == nullptr) return false;
	Vector3f32 DistVect = player->transform.GetLocalPosition() - m_pOwner->transform.GetLocalPosition();
	float distance = DistVect.Norm();
	return distance > 25.0f; // Seuil de distance
}

bool IsBlocked()
{
	return isBlocked && blockedChrono.GetElapsedTime() > blockedToggleTime;
}

bool HasCharged()
{
	return chargeChrono.GetElapsedTime() > chargeTime;
}

void OnBeginIdle() {Console::Log("Idle");}
void OnUpdateIdle()
{
	if (m_target.isSet == true)
		return;
	if (m_vPaths.Empty() == false)
		return;
	
	int patrolRangeMin = 0.0f;
	int patrolRangeMax = 15.0f;
	
	float32 x = (std::rand() % ((patrolRangeMax - patrolRangeMin) * 10 ) + patrolRangeMin * 10) / 10.0f  - patrolRangeMax * 0.5f;
	float32 y = (std::rand() % ((patrolRangeMax - patrolRangeMin) * 10 ) + patrolRangeMin * 10) / 100.0f - patrolRangeMax * 0.05f;
	float32 z = (std::rand() % ((patrolRangeMax - patrolRangeMin) * 10 ) + patrolRangeMin * 10) / 10.0f  - patrolRangeMax * 0.5f;

	Vector3f32 newPos = m_pOwner->transform.GetWorldPosition();
	newPos += m_pOwner->transform.GetLocalRight() * x * 2.0f;
	newPos += m_pOwner->transform.GetLocalUp() * y * 2.0f;
	newPos += m_pOwner->transform.GetLocalForward() * z * 2.0f;
	
	SetPath(newPos);
}
void OnEndIdle() {}

void OnBeginChase()
{
	isBlocked = false;
	Console::Log("Chase");
}
void OnUpdateChase()
{
	bool searchResult = SetPath(m_pPlayer->transform.GetWorldPosition());
	if (searchResult == false)
	{
		if (isBlocked == false)
		{
			blockedChrono.Reset();
			blockedChrono.Start();
			isBlocked = true;
		}
	}
	else
	{
		blockedChrono.Pause();
	}
}
void OnEndChase() {}

void OnBeginCharge()
{
	ResetPath();
	Console::Log("Charge");
	chargeChrono.Reset();
	chargeChrono.Start();

	Vector3f32 dir = m_pPlayer->transform.GetLocalPosition() - m_pOwner->transform.GetLocalPosition();
	finalDir = dir.Normalize();
	OrientFace(m_pPlayer->transform.GetLocalPosition());
}
void OnUpdateCharge() {}
void OnEndCharge()
{
	chargeChrono.Pause();
}

void OnBeginDash()
{
	Console::Log("Dash");
	m_speed *= 5.f;
}
void OnUpdateDash()
{
	float32 dt = GameManager::DeltaTime();
	m_pOwner->transform.WorldTranslate(finalDir * m_speed * dt);
}
void OnEndDash() {}

END_SCRIPT





#endif