#ifndef TANK_HPP_INCLUDED
#define TANK_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Enemy.hpp"
#include "GameManager.h"
#include "StateMachine/StateMachine.h"
#include "BulletTank.hpp"
#include "EnergyOrb.h"

using namespace gce;

DECLARE_CHILD_SCRIPT(Tank, Enemy, ScriptFlag::Awake | ScriptFlag::Start | ScriptFlag::SetActive | ScriptFlag::Update | ScriptFlag::CollisionEnter | ScriptFlag::Destroy)

StateMachine* m_pSm = nullptr;

bool isBlocked = false;
Chrono blockedChrono;
float32 blockedToggleTime = 3.00f;

Vector3f32 finalDir;

float32 m_shootingInterval = 3.0f;
float32 m_deltaTime = 0.0f;

Vector<BulletTank*> m_pProjectiles;

void Awake() override
{
	Enemy::Awake();
	m_speed = 4.50f;
	m_pOwner->SetName("Tank");
	m_Hp = new Health<float>(150.f);

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

	StateMachine::Condition veryClosePlayerCondition = { [this]() { return this->IsPlayerVeryClose() && !this->CheckPlayer();} };
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
	m_pSm->AddTransition(rayConditions, shooting, idle);
}

void Destroy() override
{
	GameManager::GetStateSystem().DestroyStateMachine(m_pOwner);
}

void Start() override
{
	Geometry* pGeo = GeometryFactory::LoadGeometry(RES_PATH"res/ArenaShooter/Obj/laser.obj");
	Texture* albedo = new Texture(RES_PATH"res/ArenaShooter/Obj/LaserTxtRed.png");

	for (int i = 0; i < 10; i++)
	{
		GameObject& bullet = m_pCustomScene->AddObject();
		MeshRenderer& meshProjectile = *bullet.AddComponent<MeshRenderer>();
		meshProjectile.pGeometry = pGeo;
		meshProjectile.pMaterial->albedoTextureID = albedo->GetTextureID();
		meshProjectile.pMaterial->useTextureAlbedo = 1;

		bullet.transform.SetWorldPosition({ 10.0f, 0.0f, 0.0f });
		bullet.transform.SetWorldScale({ 1.3f,1.3f,1.3f });
		bullet.SetName("Tank bullet");

		bullet.AddComponent<SphereCollider>();
		bullet.AddComponent<PhysicComponent>()->SetGravityScale(0.0f);
		m_pProjectiles.PushBack(bullet.AddScript<BulletTank>());
	}
}

void SetActiveEvent() override
{
	m_pSm->Transit("Idle");
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


void Shoot() override
{

}



bool IsPlayerClose()
{
	return m_distanceFromPlayer < 30.0f && m_distanceFromPlayer >= 25.f; // Seuil de distance
}

bool IsPlayerVeryClose()
{
	return m_distanceFromPlayer < 25.0f; // Seuil de distance
}

bool IsPlayerFar()
{
	return m_distanceFromPlayer > 40.0f; // Seuil de distance
}

bool IsBlocked()
{
	return isBlocked && blockedChrono.GetElapsedTime() > blockedToggleTime;
}

bool CheckPlayer()
{
	m_playerCheckChrono.Start();

	if (m_playerCheckChrono.GetElapsedTime() < m_playerCheckInterval)
		return false;

	Vector3f32 direction = m_pPlayer->transform.GetWorldPosition() - m_pOwner->transform.GetWorldPosition();
	direction.SelfNormalize();
	Ray ray;
	ray.origin = m_pOwner->transform.GetWorldPosition();
	ray.direction = direction;

	RaycastHit hitInfo;
	bool hit = PhysicSystem::IntersectRay(ray, hitInfo, 25.f);

	m_playerCheckChrono.Reset();

	return hit && hitInfo.pGameObject != nullptr && hitInfo.pGameObject->GetName() != "Player" && hitInfo.pGameObject->GetName() != "Tank bullet" && hitInfo.pGameObject->GetName() != "Rifle bullet" && hitInfo.pGameObject->GetName() != "Shotgun bullet" && hitInfo.pGameObject->GetName() != "Handgun bullet" && hitInfo.pGameObject != m_pOwner;
}


void OnBeginIdle()
{
}
void OnUpdateIdle()
{
	m_playerCheckChrono.Start();

	if (m_playerCheckChrono.GetElapsedTime() < m_playerCheckInterval)
		return;
	if (!m_pOwner || !m_pOwner->IsActive())
		return;
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
	m_playerCheckChrono.Reset();

}
void OnEndIdle() {}

void OnBeginChase()
{
	isBlocked = false;
	SetPath(m_pPlayer->transform.GetWorldPosition());
}
void OnUpdateChase()
{
	float32 targetDistance = (m_target.position - m_pOwner->transform.GetWorldPosition()).Norm();
	if (targetDistance < 3.0f)
		return;
	
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
	m_deltaTime += GameManager::DeltaTime();
	if (m_deltaTime >= m_shootingInterval)
	{
		OrientFace(m_pPlayer->transform.GetWorldPosition());
		BulletTank* first = m_pProjectiles[0];
		BulletTank* second = m_pProjectiles[1];
		if (!first->IsActive())
		{
			first->Init(direction, m_pOwner->transform.GetWorldPosition() + (m_pOwner->transform.GetWorldForward() * 1.f) + m_pOwner->transform.GetWorldRight() * 0.2f - m_pOwner->transform.GetWorldUp() * 0.4f, 20.f);
			m_deltaTime = 0.0f;
		}

		if (!second->IsActive())
		{
			second->Init(direction, m_pOwner->transform.GetWorldPosition() + (m_pOwner->transform.GetWorldForward() * 1.f) - m_pOwner->transform.GetWorldRight()* 0.2f - m_pOwner->transform.GetWorldUp() * 0.4f, 20.f);
			m_deltaTime = 0.0f;
		}
	}
}
void OnEndShooting()
{
}

END_SCRIPT





#endif