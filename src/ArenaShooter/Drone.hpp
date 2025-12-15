#ifndef DRONE_HPP_INCLUDED
#define DRONE_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Enemy.hpp"
#include "GameManager.h"
#include "StateMachine/StateMachine.h"
#include "BulletDrone.hpp"

using namespace gce;

DECLARE_CHILD_SCRIPT(Drone, Enemy, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::CollisionEnter)

StateMachine* m_pSm = nullptr;


bool isBlocked = false;
Chrono blockedChrono;
float32 blockedToggleTime = 3.00f;

Vector3f32 finalDir;

float32 m_shootingInterval = 3.0f;
float32 m_deltaTime = 0.0f;

Vector<BulletDrone*> m_pProjectiles;

void Awake() override
{
	Enemy::Awake();
	m_speed = 2.50f;
	m_pOwner->SetName("Drone");
	m_Hp = new Health<float>(100.f);

	m_pSm = GameManager::GetStateSystem().CreateStateMachine(m_pOwner);
	String idle = "Idle";
	m_pSm->AddAction(idle, [this]() {this->OnBeginIdle(); }, [this]() {this->OnUpdateIdle(); }, [this]() {this->OnEndIdle(); });

	String chase = "Chase";
	m_pSm->AddAction(chase, [this]() {this->OnBeginChase(); }, [this]() {this->OnUpdateChase(); }, [this]() {this->OnEndChase(); });

	String shooting = "Shooting";
	m_pSm->AddAction(shooting, [this]() {this->OnBeginShooting(); }, [this]() {this->OnUpdateShooting(); }, [this]() {this->OnEndShooting(); });


	StateMachine::Condition closePlayerCondition = { [this]() { return this->IsPlayerClose(); } };
	Vector<StateMachine::Condition> closePlayerConditions;
	closePlayerConditions.PushBack(closePlayerCondition);
	m_pSm->AddTransition(closePlayerConditions, shooting, chase);
	m_pSm->AddTransition(closePlayerConditions, idle, chase);

	StateMachine::Condition veryClosePlayerCondition = { [this]() { return this->IsPlayerVeryClose(); } };
	Vector<StateMachine::Condition> veryClosePlayerConditions;
	veryClosePlayerConditions.PushBack(veryClosePlayerCondition);
	m_pSm->AddTransition(veryClosePlayerConditions, chase, shooting);

	StateMachine::Condition farPlayerCondition = { [this]() { return this->IsPlayerFar(); } };
	Vector<StateMachine::Condition> farPlayerConditions;
	farPlayerConditions.PushBack(farPlayerCondition);
	m_pSm->AddTransition(farPlayerConditions, chase, idle);

	StateMachine::Condition blockedCondition = { [this]() { return this->IsBlocked(); } };
	Vector<StateMachine::Condition> blockedConditions;
	blockedConditions.PushBack(blockedCondition);
	m_pSm->AddTransition(blockedConditions, chase, idle);

	StateMachine::Condition rayCondition = { [this]() { return this->CheckPlayer(); } };
	Vector<StateMachine::Condition> rayConditions;
	rayConditions.PushBack(rayCondition);
	m_pSm->AddTransition(rayConditions, shooting, chase);


	for (int i = 0; i < 10; i++)
	{
		GameObject& bullet = GameObject::Create(m_pOwner->GetScene());
		MeshRenderer& meshProjectile = *bullet.AddComponent<MeshRenderer>();
		meshProjectile.pGeometry = SHAPES.SPHERE;
		bullet.transform.SetWorldPosition({ 10.0f, 0.0f, 0.0f });
		bullet.transform.SetWorldScale({ 0.3f,0.3f,0.3f });
		bullet.SetName("Drone bullet");

		bullet.AddComponent<SphereCollider>();
		bullet.AddComponent<PhysicComponent>()->SetGravityScale(0.0f);
		m_pProjectiles.PushBack(bullet.AddScript<BulletDrone>());
	}
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

void CollisionEnter(GameObject* other) override
{

}

bool IsPlayerClose()
{
	GameObject* player = m_pPlayer;
	if (player == nullptr) return false;
	Vector3f32 DistVect = player->transform.GetLocalPosition() - m_pOwner->transform.GetLocalPosition();
	float distance = DistVect.Norm();
	return distance < 25.0f && distance >= 20.f; // Seuil de distance
}

bool IsPlayerVeryClose()
{
	GameObject* player = m_pPlayer;
	if (player == nullptr) return false;
	Vector3f32 DistVect = player->transform.GetLocalPosition() - m_pOwner->transform.GetLocalPosition();
	float distance = DistVect.Norm();
	return distance < 20.0f; // Seuil de distance
}

bool IsPlayerFar()
{
	GameObject* player = m_pPlayer;
	if (player == nullptr) return false;
	Vector3f32 DistVect = player->transform.GetLocalPosition() - m_pOwner->transform.GetLocalPosition();
	float distance = DistVect.Norm();
	return distance > 30.0f; // Seuil de distance
}

bool IsBlocked()
{
	return isBlocked && blockedChrono.GetElapsedTime() > blockedToggleTime;
}

bool CheckPlayer()
{
	Vector3f32 direction = m_pPlayer->transform.GetWorldPosition() - m_pOwner->transform.GetWorldPosition();
	direction.SelfNormalize();
	Ray ray;
	ray.origin = m_pOwner->transform.GetWorldPosition();
	ray.direction = direction;

	RaycastHit hitInfo;
	bool hit = PhysicSystem::IntersectRay(ray, hitInfo, 25.f);

	return hit && hitInfo.pGameObject != nullptr && hitInfo.pGameObject->GetName() != "Player" && hitInfo.pGameObject != m_pOwner;
}


void OnBeginIdle() { Console::Log("Idle"); }
void OnUpdateIdle()
{
	if (m_target.isSet == true)
		return;
	if (m_vPaths.Empty() == false)
		return;

	int patrolRangeMin = 0.0f;
	int patrolRangeMax = 15.0f;

	float32 x = (std::rand() % ((patrolRangeMax - patrolRangeMin) * 10) + patrolRangeMin * 10) / 10.0f - patrolRangeMax * 0.5f;
	float32 y = (std::rand() % ((patrolRangeMax - patrolRangeMin) * 10) + patrolRangeMin * 10) / 100.0f - patrolRangeMax * 0.05f;
	float32 z = (std::rand() % ((patrolRangeMax - patrolRangeMin) * 10) + patrolRangeMin * 10) / 10.0f - patrolRangeMax * 0.5f;

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

void OnBeginShooting()
{
	m_deltaTime = 0.0f;
	ResetPath();
}
void OnUpdateShooting()
{
	Vector3f32 direction = m_pPlayer->transform.GetWorldPosition() - m_pOwner->transform.GetWorldPosition();
	direction.SelfNormalize();
	OrientFace(m_pPlayer->transform.GetWorldPosition());
	m_deltaTime += GameManager::DeltaTime();
	if (m_deltaTime >= m_shootingInterval)
	{
		BulletDrone* first = m_pProjectiles[0];
		if (!first->IsActive())
			first->Init(direction, m_pOwner->transform.GetWorldPosition() + m_pOwner->transform.GetWorldForward() * 1.f, 20.f);
	}
}
void OnEndShooting()
{
}

END_SCRIPT





#endif