#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include <iostream>

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Core/Maths/Vector3.h"
#include "GameManager.h"
#include "Shapes.h"
#include "Rifle.hpp"
#include "Shotgun.hpp"
#include "Handgun.hpp"
#include "Health.h"
#include "WeaponController.hpp"
#include "BulletDrone.hpp"
#include "BulletTank.hpp"
#include "SceneManager.h"
#include "CustomScene.h"

using namespace gce;

DECLARE_SCRIPT(Player, ScriptFlag::Awake | ScriptFlag::Update | ScriptFlag::CollisionStay | ScriptFlag::CollisionEnter | ScriptFlag::CollisionExit)

float32 m_speed = 5;
float32 m_jumpForce = 40000;
float32 m_boostForce = 500;
float32 m_airMovementForce = m_jumpForce / 15;
Vector3f32 m_currentOffset = { 0,0,0 };
Camera* m_camera = nullptr;
Rifle* m_rifle = nullptr;
Shotgun* m_shotgun = nullptr;
Handgun* m_handgun = nullptr;

WeaponController* m_weaponController = nullptr;

Health<int>* m_health = nullptr;


void Awake() override
{
	m_health = new Health<int>(5);
	
	Geometry* pShotgunGeo = GeometryFactory::LoadGeometry(RES_PATH"res/ArenaShooter/Obj/shotgun_lower.obj");

	GameObject& cam = GameObject::Create(m_pOwner->GetScene());
	cam.SetParent(*m_pOwner);
	cam.transform.SetLocalPosition({ 0.f, 0.8f, 0.f });
	m_camera = cam.AddComponent<Camera>();
	m_camera->SetMainCamera();
	m_camera->SetType(PERSPECTIVE);
	m_camera->perspective.fov = XM_PIDIV4;
	m_camera->perspective.nearPlane = 0.001f;
	m_camera->perspective.farPlane = 500.0f;
	m_camera->perspective.aspectRatio = 600.0f / 400.0f;
	m_camera->perspective.up = { 0.0f, 1.0f, 0.0f };

	GameObject& weaponControllerObj = GameObject::Create(m_pOwner->GetScene());
	m_weaponController = weaponControllerObj.AddScript<WeaponController>();
	weaponControllerObj.SetParent(*m_pOwner);

	GameObject& rifle = GameObject::Create(m_pOwner->GetScene());
	MeshRenderer& meshProjectileRifle = *rifle.AddComponent<MeshRenderer>();
	meshProjectileRifle.pGeometry = SHAPES.SPHERE;
	m_rifle = rifle.AddScript<Rifle>();
	rifle.transform.SetWorldScale({ 0.3f,0.3f,0.3f });
	rifle.SetParent(cam);
	rifle.transform.SetLocalPosition({ 0.3f,-0.3f,1.f });
	m_weaponController->AddWeapon(m_rifle);

	GameObject& shotgun = GameObject::Create(m_pOwner->GetScene());
	MeshRenderer& meshProjectileShotgun = *shotgun.AddComponent<MeshRenderer>();
	meshProjectileShotgun.pGeometry = pShotgunGeo;
	m_shotgun = shotgun.AddScript<Shotgun>();
	shotgun.transform.SetWorldScale({ 0.2f,0.2f,0.2f });
	shotgun.SetParent(cam);
	shotgun.transform.SetLocalPosition({ 0.8f,-1.f,0.3f });
	shotgun.transform.SetLocalRotation({ 0.f, 0.f, 0.f });
	m_weaponController->AddWeapon(m_shotgun);

	GameObject& handgun = GameObject::Create(m_pOwner->GetScene());
	MeshRenderer& meshProjectileHandgun = *handgun.AddComponent<MeshRenderer>();
	meshProjectileHandgun.pGeometry = SHAPES.CUBE;
	m_handgun = handgun.AddScript<Handgun>();
	handgun.transform.SetWorldScale({ 0.3f,0.3f,0.3f });
	handgun.SetParent(cam);
	handgun.transform.SetLocalPosition({ 0.3f,-0.3f,1.f });
	m_weaponController->AddWeapon(m_handgun);
}

void Test()
{
	GameObject& testObject = GameObject::Create(m_pOwner->GetScene());
	testObject.AddComponent<MeshRenderer>()->pGeometry = SHAPES.CUBE;

	testObject.transform.SetWorldPosition(m_pOwner->transform.GetWorldPosition() + m_pOwner->transform.GetWorldForward() * 2.f);
	
}

void Update() override
{
	m_deltaTime = GameManager::DeltaTime();
	RaycastUpdate();
	if (m_health->GetHealth() <= 0)
	{
		m_pOwner->SetActive(false);
		SceneManager::GetInstance()->GetCurrentScene()->Empty(1);
		SceneManager::GetInstance()->ChangeScene(GAMEOVER);
		
	}
}

bool IsRising()
{
	if (m_pOwner->GetComponent<PhysicComponent>()->GetVelocity().y <= 0)
		return false;
	else
		return true;
}

bool IsGrounded()
{
	if (m_isGrounded == true)
		return true;
	else if (m_isGrounded == false)
		return false;
}

void Jump()
{
	if (m_isGrounded)
	{
		Force jumpForce;
		jumpForce.direction = { 0, 1, 0};
		jumpForce.norm = m_jumpForce;
		
		Vector3f32 jumpDirection;
		jumpDirection = { m_currentOffset.x, 0, m_currentOffset.z };
		jumpForce.direction += jumpDirection;

		m_pOwner->GetComponent<PhysicComponent>()->AddForce(jumpForce);
	}
}

void BoostUp()
{
	if (m_isGrounded == false)
	{
		Force boostForce;
		boostForce.direction = { 0, 1, 0 };
		boostForce.norm = m_boostForce;

		m_pOwner->GetComponent<PhysicComponent>()->AddForce(boostForce);
	}
}

void Move(Vector3f32 direction)
{
	m_currentOffset = (m_pOwner->transform.GetLocalForward().Normalize() * direction.z + m_pOwner->transform.GetLocalRight().Normalize() * direction.x);
	Vector3f32 offset = m_currentOffset * m_speed;

	if(m_isGrounded == true)
	{
		PhysicComponent& phys = *m_pOwner->GetComponent<PhysicComponent>();
		Vector3f32 vel = phys.GetVelocity();
		phys.SetVelocity({ offset.x, vel.y , offset.z });
	}
	else if (m_isGrounded == false)
	{
		Force airMovementForce;
		airMovementForce.direction = m_currentOffset;
		airMovementForce.norm = m_airMovementForce;

		PhysicComponent& phys = *m_pOwner->GetComponent<PhysicComponent>();
		Vector3f32 vel = phys.GetVelocity();

		if (m_currentOffset.x > 0 && vel.x > offset.x )
			airMovementForce.direction.x = 0;
		else if (m_currentOffset.x < 0 && vel.x < offset.x)
			airMovementForce.direction.x = 0;
		if (m_currentOffset.z > 0 && vel.z > offset.z)
			airMovementForce.direction.z = 0;
		else if (m_currentOffset.z < 0 && vel.z < offset.z)
			airMovementForce.direction.z = 0;

		m_pOwner->GetComponent<PhysicComponent>()->AddForce(airMovementForce);
	}
}

void Rotate(Vector3f32 rotation)
{
	Vector3f32 tilt = rotation * m_deltaTime;

	m_pOwner->transform.LocalRotate(tilt);
}

void Die()
{
	m_health->TakeDamage(5);
}

void CollisionStay(GameObject* other) override
{
	
}

void CollisionEnter(GameObject* other)
{
	/*Console::Log("Touch");
	Console::Log(other->GetName());

	m_isGrounded = true;*/

	if (other->GetScript<BulletDrone>())
	{
		m_health->TakeDamage(other->GetScript<BulletDrone>()->GetDmgBullet());
	}
	if (other->GetScript<BulletTank>())
	{
		m_health->TakeDamage(other->GetScript<BulletTank>()->GetDmgBullet());
	}
	
}

void CollisionExit(GameObject* other) override
{
	/*Console::Log("Untouch");
	
	m_isGrounded = false;*/
}

WeaponController* GetWeaponController()
{
	return m_weaponController;
}

void RaycastUpdate()
{
	//Texture laserTexture(RES_PATH"res/Textures/rouge-laser.jpg");
	Ray ray;
	ray.origin = m_pOwner->transform.GetWorldPosition();
	ray.direction = m_pOwner->transform.GetWorldUp();
	ray.direction.y *= -1.f;

	float32 maxDistance = 1.f;
	RaycastHit hitInfo;
	float32 distance = maxDistance;
	Vector3f32 hitPoint = ray.origin + ray.direction * distance;
	if (PhysicSystem::IntersectRay(ray, hitInfo, maxDistance))
	{
		if (hitInfo.pGameObject && hitInfo.pGameObject->HasComponent<MeshRenderer>())
		{
			//hitInfo.pGameObject->GetComponent<MeshRenderer>()->pMaterial->albedoTextureID = laserTexture.GetTextureID();
		}

		distance = hitInfo.distance;
		hitPoint = hitInfo.point;


		m_isGrounded = true;
	}
	else
	{
		m_isGrounded = false;
	}
}

private:
	float32 m_deltaTime;

	bool m_isGrounded;


END_SCRIPT

#endif // !PLAYER_HPP_DEFINED
