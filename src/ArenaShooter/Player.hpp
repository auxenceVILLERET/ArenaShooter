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

using namespace gce;

DECLARE_SCRIPT(Player, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::CollisionStay | ScriptFlag::CollisionEnter | ScriptFlag::CollisionExit)

float32 m_speed = 5;
float32 m_jumpForce = 50000;
float32 m_airMovementForce = m_jumpForce / 10;
Camera* m_camera = nullptr;
Rifle* m_rifle = nullptr;
Shotgun* m_shotgun = nullptr;

Vector3f32 m_currentOffset = {0,0,0};

void Init(D12PipelineObject* pPso)
{
	m_pOwner->AddComponent<BoxCollider>();
	m_pOwner->AddComponent<PhysicComponent>()->SetMass(80.0f);
	m_pOwner->GetComponent<PhysicComponent>()->SetBounciness(0.0f);
	m_pOwner->transform.SetWorldPosition({ 0,3,0 });
	m_pOwner->transform.SetWorldScale({ 1.f, 1.f, 1.f });
	
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
	

	GameObject& rifle = GameObject::Create(m_pOwner->GetScene());
	m_rifle = rifle.AddScript<Rifle>();
	m_rifle->Init(pPso);
	rifle.transform.SetWorldScale({ 0.3f,0.3f,0.3f });
	rifle.SetParent(cam);
	rifle.transform.SetLocalPosition({ 0.3f,-0.3f,1.f });

	/*GameObject& shotgun = GameObject::Create(m_pOwner->GetScene());
	m_shotgun = shotgun.AddScript<Shotgun>();
	m_shotgun->Init(pPso);
	shotgun.transform.SetWorldScale({ 0.3f,0.3f,0.3f });
	shotgun.SetParent(cam);
	shotgun.transform.SetLocalPosition({ 0.3f,-0.3f,1.f });*/

}

void Start() override
{
	
}


void Update() override
{
	m_deltaTime = GameManager::DeltaTime();
}

bool IsRising()
{
	if (m_pOwner->GetComponent<PhysicComponent>()->GetVelocity().y <= 0)
		return false;
	else
		return true;
}

bool IsAirborne()
{
	//if (m_pOwner->transform.GetWorldPosition().y <= 0.5f)
	{
		Force land;
		land.direction = (m_currentOffset * m_speed).Normalize();
		land.norm = m_jumpForce;

		return false;
		m_pOwner->GetComponent<PhysicComponent>()->AddForce(land);
	}
	/*else*/
	{
		return true;
	}
}

bool IsGrounded()
{
	if (m_isGrounded)
	{
		m_isGrounded = false;
		return true;
	}	
	else
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

void Move(Vector3f32 direction)
{
	m_currentOffset = (m_pOwner->transform.GetLocalForward().Normalize() * direction.z + m_pOwner->transform.GetLocalRight().Normalize() * direction.x);
	Vector3f32 offset = m_currentOffset * m_speed;

	if(m_isGrounded == true)
	{

		PhysicComponent& phys = *m_pOwner->GetComponent<PhysicComponent>();
		Vector3f32 vel = phys.GetVelocity();
		phys.SetVelocity({ offset.x, vel.y, offset.z });
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

}

void CollisionStay(GameObject* other) override
{
	
}

void CollisionEnter(GameObject* other)
{
	Console::Log("Touch");
	if (other->GetName() == "Ground" && other->transform.GetWorldPosition().y < m_pOwner->transform.GetWorldPosition().y)
	{
		m_isGrounded = true;
		Console::Log(other->transform.GetWorldPosition().y);
		Console::Log(m_pOwner->transform.GetWorldPosition().y);
	}
}

void CollisionExit(GameObject* other) override
{
	Console::Log("Untouch");
	if (other->GetName() == "Ground" && other->transform.GetWorldPosition().y < m_pOwner->transform.GetWorldPosition().y)
	{
		m_isGrounded = false;
	}
}

private:
	float32 m_deltaTime;

	bool m_isGrounded;


END_SCRIPT

#endif // !PLAYER_HPP_DEFINED
