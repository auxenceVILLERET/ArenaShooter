#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include <iostream>

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Core/Maths/Vector3.h"
#include "GameManager.h"
#include "Shapes.h"

using namespace gce;

DECLARE_SCRIPT(PlayerMovement, ScriptFlag::Start | ScriptFlag::Update)


float32 m_speed = 5;
float32 m_jumpForce = 15000;
Camera* m_camera = nullptr;

Vector3f32 m_moveOffset = {0,0,0};

void Init(D12PipelineObject* pPso)
{
	MeshRenderer& meshPlayer = *m_pOwner->AddComponent<MeshRenderer>();
	meshPlayer.pGeometry = SHAPES.CUBE;
	meshPlayer.pPso = pPso;
	
	m_pOwner->transform.SetWorldPosition({ 0,2,0 });
	m_pOwner->transform.SetWorldScale({ 0.8f,1.7f,0.8f });
	m_pOwner->AddComponent<BoxCollider>();
	m_pOwner->AddComponent<PhysicComponent>();
	m_pOwner->GetComponent<PhysicComponent>()->SetBounciness(0.0f);
	
	GameObject& cam = GameObject::Create(m_pOwner->GetScene());
	m_camera = cam.AddComponent<Camera>();
	m_camera->SetMainCamera();
	m_camera->SetType(PERSPECTIVE);
	m_camera->perspective.fov = XM_PIDIV4;
	m_camera->perspective.nearPlane = 0.001f;
	m_camera->perspective.farPlane = 500.0f;
	m_camera->perspective.aspectRatio = 600.0f / 400.0f;
	m_camera->perspective.up = { 0.0f, 1.0f, 0.0f };
	cam.SetParent(*m_pOwner);
	cam.transform.SetLocalPosition({0.f, 0.8f, 0.f});
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
		
		Vector3f32 jumpDirection = (m_moveOffset * m_speed).Normalize();
		jumpForce.direction += jumpDirection;

		m_pOwner->GetComponent<PhysicComponent>()->AddForce(jumpForce);

		
	}
}

void Move(Vector3f32 direction)
{

	Vector3f32 offset;

	if (IsAirborne() == false)
	{
		offset = (m_pOwner->transform.GetLocalForward().Normalize() * direction.z + m_pOwner->transform.GetLocalRight().Normalize() * direction.x) * m_speed;
		offset *= m_deltaTime;

		m_pOwner->transform.LocalTranslate(offset);
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

void MovingForward(float32 direction)
{
	m_moveOffset.z = direction;
}
void MovingBackward(float32 direction)
{
	m_moveOffset.z = direction;
}
void MovingLeft(float32 direction)
{
	m_moveOffset.x = direction;
}
void MovingRight(float32 direction)
{
	m_moveOffset.x = direction;
}


private:
	float32 m_deltaTime;


END_SCRIPT

#endif // !PLAYER_HPP_DEFINED
