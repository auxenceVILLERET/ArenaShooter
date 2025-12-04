#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Core/Maths/Vector3.h"
#include "GameManager.h"

using namespace gce;

DECLARE_SCRIPT(PlayerMovement, ScriptFlag::Start | ScriptFlag::Update)


float32 m_speed = 5;
float32 m_jumpForce = 100;
float32 m_deltaTime;

void Start() override
{
	m_pOwner->transform.SetWorldPosition({ 0,2,0 });
	m_pOwner->transform.SetWorldScale({ 0.8f,1.7f,0.8f });
	m_pOwner->AddComponent<BoxCollider>();
	m_pOwner->AddComponent<PhysicComponent>();
	
	GameObject& cam = GameObject::Create(m_pOwner->GetScene());
	Camera& camera = *cam.AddComponent<Camera>();
	camera.SetMainCamera();
	camera.SetType(PERSPECTIVE);
	camera.perspective.fov = XM_PIDIV4;
	camera.perspective.nearPlane = 0.001f;
	camera.perspective.farPlane = 500.0f;
	camera.perspective.aspectRatio = 600.0f / 400.0f;
	camera.perspective.up = { 0.0f, 1.0f, 0.0f };
	cam.SetParent(*m_pOwner);
	cam.transform.SetLocalPosition({0.f, 0.8f, 0.f});
}


void Update() override
{
	m_deltaTime = GameManager::DeltaTime();
}

void Jump()
{
}

void Move(Vector3f32 direction)
{
	Vector3f32 offset = direction * m_speed;
	offset *= m_deltaTime;

	m_pOwner->transform.LocalTranslate(offset);
}

void Die()
{

}


END_SCRIPT

#endif // !PLAYER_HPP_DEFINED
