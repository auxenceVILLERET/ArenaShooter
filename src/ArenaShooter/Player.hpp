#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include <iostream>

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Core/Maths/Vector3.h"
#include "GameManager.h"

using namespace gce;

DECLARE_SCRIPT(PlayerMovement, ScriptFlag::Start | ScriptFlag::Update)


float32 m_speed = 5;
float32 m_jumpForce = 15000;
float32 m_deltaTime;

void Start() override
{
	m_pOwner->transform.SetWorldPosition({ 0,1,0 });
	m_pOwner->transform.SetWorldScale({ 1,1,1 });
	m_pOwner->AddComponent<BoxCollider>();
	m_pOwner->AddComponent<PhysicComponent>();
	m_pOwner->GetComponent<PhysicComponent>()->SetBounciness(0.0f);
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
	if (m_pOwner->transform.GetWorldPosition().y <= 0.f)
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
	Vector3f32 offset = direction * m_speed;
	offset *= m_deltaTime;

	if (IsAirborne())
		offset *= 0.25f;

	m_pOwner->transform.LocalTranslate(offset);
}

void Die()
{

}


END_SCRIPT

#endif // !PLAYER_HPP_DEFINED
