#ifndef BOSS_HPP_INCLUDED
#define BOSS_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Enemy.hpp"
#include "GameManager.h"
#include "StateMachine/StateMachine.h"
#include "BulletBoss.hpp"
#include "EnergyOrb.hpp"

using namespace gce;

DECLARE_CHILD_SCRIPT(Boss, Enemy, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::CollisionEnter)

StateMachine* m_pSm = nullptr;


bool isBlocked = false;
Chrono blockedChrono;
float32 blockedToggleTime = 3.00f;

Vector3f32 finalDir;

float32 m_shootingInterval = 3.0f;
float32 m_punchingInterval = 1.5f;
float32 m_deltaTime = 0.0f;

Vector<BulletBoss*> m_pProjectiles;

float32 m_punchUpForce = 5000.f;
float32 m_punchForce = 200000.f;


void Awake() override
{
	Enemy::Awake();
	m_speed = 2.f;
	m_pOwner->SetName("Boss");
	m_Hp = new Health<float>(300.f);

	m_pSm = GameManager::GetStateSystem().CreateStateMachine(m_pOwner);
	String idle = "Idle";
	m_pSm->AddAction(idle, [this]() {this->OnBeginIdle(); }, [this]() {this->OnUpdateIdle(); }, [this]() {this->OnEndIdle(); });

	String punching = "Punching";
	m_pSm->AddAction(punching, [this]() {this->OnBeginPunching(); }, [this]() {this->OnUpdatePunching(); }, [this]() {this->OnEndPunching(); });

	StateMachine::Condition closePlayerCondition = { [this]() { return this->IsPlayerClose() && this->CheckPlayer() == false; } };
	Vector<StateMachine::Condition> closePlayerConditions;
	closePlayerConditions.PushBack(closePlayerCondition);
	m_pSm->AddTransition(closePlayerConditions, idle, punching);

	StateMachine::Condition farPlayerCondition = { [this]() { return this->IsPlayerFar(); } };
	Vector<StateMachine::Condition> farPlayerConditions;
	farPlayerConditions.PushBack(farPlayerCondition);
	m_pSm->AddTransition(farPlayerConditions, punching, idle);

	/*StateMachine::Condition rayCondition = { [this]() { return this->CheckPlayer(); } };
	Vector<StateMachine::Condition> rayConditions;
	rayConditions.PushBack(rayCondition);
	m_pSm->AddTransition(rayConditions, shooting, idle);*/


	for (int i = 0; i < 10; i++)
	{
		GameObject& bullet = GameObject::Create(m_pOwner->GetScene());
		MeshRenderer& meshProjectile = *bullet.AddComponent<MeshRenderer>();
		meshProjectile.pGeometry = SHAPES.CYLINDER;
		bullet.transform.SetWorldPosition({ 10.0f, 0.0f, 0.0f });
		bullet.transform.SetWorldScale({ 1.f,5.f,1.f });
		bullet.SetName("Boss bullet");

		bullet.AddComponent<BoxCollider>();
		bullet.AddComponent<PhysicComponent>()->SetGravityScale(0.0f);
		m_pProjectiles.PushBack(bullet.AddScript<BulletBoss>());
	}
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
	GameObject* player = m_pPlayer;
	if (player == nullptr) return false;
	Vector3f32 DistVect = player->transform.GetLocalPosition() - m_pOwner->transform.GetLocalPosition();
	float distance = DistVect.Norm();
	return distance <= 15.f; // Seuil de distance
}


bool IsPlayerFar()
{
	GameObject* player = m_pPlayer;
	if (player == nullptr) return false;
	Vector3f32 DistVect = player->transform.GetLocalPosition() - m_pOwner->transform.GetLocalPosition();
	float distance = DistVect.Norm();
	return distance > 15.f; // Seuil de distance
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

	return hit && hitInfo.pGameObject != nullptr && hitInfo.pGameObject->GetName() != "Player" && hitInfo.pGameObject->GetName() != "Boss bullet" && hitInfo.pGameObject->GetName() != "Rifle bullet" && hitInfo.pGameObject->GetName() != "Shotgun bullet" && hitInfo.pGameObject->GetName() != "Handgun bullet" && hitInfo.pGameObject != m_pOwner;
}

void ShootPattern1(float32 sideOffset)
{
	Vector3f32 shootFromOffset = m_pOwner->transform.GetWorldPosition() + m_pOwner->transform.GetWorldForward() * 1.f;
	shootFromOffset.y += 2;
	
	Vector3f32 shotDirOffset = { m_pPlayer->transform.GetWorldPosition() - m_pOwner->transform.GetWorldPosition() };
	shotDirOffset.y = 0.f;
	shotDirOffset.SelfNormalize();
	
	Vector3f32 right = { -shotDirOffset.z, 0.f, shotDirOffset.x };
	float playerSideOffset = sideOffset;
	float playerSideOffset2 = playerSideOffset * 2.f;
	
	// Cibles décalées
	Vector3f32 targetLeft = m_pPlayer->transform.GetWorldPosition() - right * playerSideOffset;
	Vector3f32 targetRight = m_pPlayer->transform.GetWorldPosition() + right * playerSideOffset;
	Vector3f32 targetLeft2 = m_pPlayer->transform.GetWorldPosition() - right * playerSideOffset2;
	Vector3f32 targetRight2 = m_pPlayer->transform.GetWorldPosition() + right * playerSideOffset2;

	// Directions finales
	Vector3f32 shotDirLeft = (targetLeft - shootFromOffset).SelfNormalize();
	shotDirLeft.y = 0.f;
	Vector3f32 shotDirRight = (targetRight - shootFromOffset).SelfNormalize();
	shotDirRight.y = 0.f;
	Vector3f32 shotDirLeft2 = (targetLeft2 - shootFromOffset).SelfNormalize();
	shotDirLeft2.y = 0.f;
	Vector3f32 shotDirRight2 = (targetRight2 - shootFromOffset).SelfNormalize();
	shotDirRight2.y = 0.f;

	for (BulletBoss* first : m_pProjectiles)
	{
		if (!first->IsActive())
		{
			Vector3f32 shootFrom = shootFromOffset;
			Vector3f32 shotDir = shotDirOffset;
			first->Init(shotDir, shootFrom, 15.f);
			Console::Log("Boss Shot");
			break;
		}
	}

	for (BulletBoss* second : m_pProjectiles)
	{
		if (!second->IsActive())
		{
			Vector3f32 shootFrom = shootFromOffset;
			second->Init(shotDirLeft, shootFrom, 15.f);
			Console::Log("Boss Shot");
			break;
		}
	}
	for (BulletBoss* third : m_pProjectiles)
	{
		if (!third->IsActive())
		{
			Vector3f32 shootFrom = shootFromOffset;
			third->Init(shotDirRight, shootFrom, 15.f);
			Console::Log("Boss Shot");
			break;
		}
	}
	for (BulletBoss* fourth : m_pProjectiles)
	{
		if (!fourth->IsActive())
		{
			Vector3f32 shootFrom = shootFromOffset;
			fourth->Init(shotDirLeft2, shootFrom, 15.f);
			Console::Log("Boss Shot");
			break;
		}
	}
	for (BulletBoss* fifth : m_pProjectiles)
	{
		if (!fifth->IsActive())
		{
			Vector3f32 shootFrom = shootFromOffset;
			fifth->Init(shotDirRight2, shootFrom, 15.f);
			Console::Log("Boss Shot");
			break;
		}
	}
}

void OnBeginIdle() 
{
	Console::Log("Boss Idle");
	m_deltaTime = 0.0f;
	ResetPath();
}
void OnUpdateIdle()
{
	m_deltaTime += GameManager::DeltaTime();
	if (m_deltaTime >= m_shootingInterval)
	{
		ShootPattern1(10.f);
		m_deltaTime = 0.0f;
	}
	Console::Log(m_Hp->GetHealth());
}
void OnEndIdle() {}

void OnBeginPunching()
{
	Console::Log("Boss Punching");
	m_deltaTime = 0.0f;
	ResetPath();
}
void OnUpdatePunching()
{
	Vector3f32 direction = m_pPlayer->transform.GetWorldPosition() - m_pOwner->transform.GetWorldPosition();
	direction.SelfNormalize();
	direction.y = 0.f; 
	m_deltaTime += GameManager::DeltaTime();
	if (m_deltaTime >= m_punchingInterval)
	{
		Force punchUp;
		punchUp.direction = Vector3f32(0.f, 1.f, 0.f); // Add some upward force
		punchUp.norm = m_punchUpForce;
		m_pPlayer->GetComponent<PhysicComponent>()->AddForce(punchUp);

		m_pPlayer->GetScript<Player>()->GettingPunched();

		if (m_pPlayer->GetScript<Player>()->IsGrounded() == false)
		{
			Force punch;
			punch.direction = direction;
			punch.norm = m_punchForce;
			m_pPlayer->GetComponent<PhysicComponent>()->AddForce(punch);
			m_deltaTime = 0.0f;
			Console::Log("Boss Punched");
		}
	}

}
void OnEndPunching()
{
}

END_SCRIPT





#endif