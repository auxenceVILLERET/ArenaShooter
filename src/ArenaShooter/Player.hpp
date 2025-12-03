#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Core/Maths/Vector3.h"
#include "GameManager.h"

using namespace gce;

DECLARE_SCRIPT(PlayerMovement, ScriptFlag::Start | ScriptFlag::Update)


float32 m_speed = 10;
float32 m_deltaTime;

void Start() override
{
	m_pOwner->transform.SetWorldPosition({ 0,1,0 });
	m_pOwner->transform.SetWorldScale({ 1,1,1 });
	m_pOwner->AddComponent<BoxCollider>();
	m_pOwner->AddComponent<PhysicComponent>();

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
