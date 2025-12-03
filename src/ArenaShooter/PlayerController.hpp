#ifndef PLAYERCONTROLLER_HPP_INCLUDED
#define PLAYERCONTROLLER_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Core/Maths/Vector3.h"
#include "GameManager.h"
#include "Rifle.hpp"

#include "Player.hpp"
#include "InputsMethods.h"

using namespace gce;

DECLARE_SCRIPT(PlayerController, ScriptFlag::Start | ScriptFlag::Update)


float32 m_deltaTime;

GameObject* m_pPlayer;

Keyboard::Key m_keyForward = Keyboard::Z;
Keyboard::Key m_keyBackward = Keyboard::S;
Keyboard::Key m_keyLeft = Keyboard::Q;
Keyboard::Key m_keyRight = Keyboard::D;

Keyboard::Key m_keyJump = Keyboard::SPACE;
Keyboard::Key m_keyReload = Keyboard::R;

Mouse::Button m_buttonLeft = Mouse::LEFT;

void Start() override
{
	m_pPlayer = m_pOwner;
}

void Update() override
{
	m_deltaTime = GameManager::DeltaTime();
	HandleInput(GameManager::DeltaTime());
}

void HandleInput(float32 deltaTime)
{
	if (GetKey(m_keyForward))
		Move({ 0, 0, 1 });
	if (GetKey(m_keyBackward))
		Move({ 0, 0, -1 });
	if (GetKey(m_keyLeft))
		Move({ -1, 0, 0 });
	if (GetKey(m_keyRight))
		Move({ 1, 0, 0 });

	if (GetKeyDown(m_keyJump))
		m_pPlayer->GetScript<PlayerMovement>()->Jump();
	
	if (GetKeyDown(m_keyReload))
		m_pPlayer->GetScript<Rifle>()->Reload();

	if (GetButton(m_buttonLeft))
		m_pPlayer->GetScript<Rifle>()->BeginShot();

}

void Move(Vector3f32 direction)
{
	m_pPlayer->GetScript<PlayerMovement>()->Move(direction);
}

END_SCRIPT

#endif // !PLAYERCONTROLLER_HPP_DEFINED
