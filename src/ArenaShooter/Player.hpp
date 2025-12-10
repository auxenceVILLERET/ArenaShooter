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

DECLARE_SCRIPT(Player, ScriptFlag::Awake | ScriptFlag::Update)

float32 m_speed = 5;
float32 m_jumpForce = 15000;
Camera* m_camera = nullptr;
Rifle* m_rifle = nullptr;
Shotgun* m_shotgun = nullptr;

void Awake() override
{

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


	// GameObject& rifle = GameObject::Create(m_pOwner->GetScene());
	// m_rifle = rifle.AddScript<Rifle>();
	// rifle.transform.SetWorldScale({ 0.3f,0.3f,0.3f });
	// rifle.SetParent(cam);
	// rifle.transform.SetLocalPosition({ 0.3f,-0.3f,1.f });

	GameObject& shotgun = GameObject::Create(m_pOwner->GetScene());
	m_shotgun = shotgun.AddScript<Shotgun>();
	shotgun.transform.SetWorldScale({ 0.3f,0.3f,0.3f });
	shotgun.SetParent(cam);
	shotgun.transform.SetLocalPosition({ 0.3f,-0.3f,1.f });

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
	if (m_pOwner->transform.GetWorldPosition().y <= 0.5f)
		return false;
	else
		return true;
}

void Jump()
{
	if (IsAirborne() == false && IsRising() == false)
	{
		Force jumpForce;
		jumpForce.direction = { 0, 1, 0 };
		jumpForce.norm = m_jumpForce;

		m_pOwner->GetComponent<PhysicComponent>()->AddForce(jumpForce);
	}
}

void Move(Vector3f32 direction)
{
	Vector3f32 offset = (m_pOwner->transform.GetLocalForward().Normalize() * direction.z + m_pOwner->transform.GetLocalRight().Normalize() * direction.x) * m_speed;
	
	PhysicComponent& phys = *m_pOwner->GetComponent<PhysicComponent>();
	Vector3f32 vel = phys.GetVelocity();
	phys.SetVelocity({offset.x, vel.y, offset.z} );

	// if (IsAirborne())
	// 	offset *= 0.25f;

	// m_pOwner->transform.LocalTranslate(offset);
}

void Rotate(Vector3f32 rotation)
{
	Vector3f32 tilt = rotation * m_deltaTime;

	m_pOwner->transform.LocalRotate(tilt);
}

void Die()
{

}



private:
	float32 m_deltaTime;


END_SCRIPT

#endif // !PLAYER_HPP_DEFINED
