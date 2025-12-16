#ifndef PLAYERCONTROLLER_HPP_INCLUDED
#define PLAYERCONTROLLER_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Core/Maths/Vector3.h"
#include "GameManager.h"
#include "Rifle.hpp"
#include "WeaponController.hpp"

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
Keyboard::Key m_keyEscape = Keyboard::ESCAPE;

Keyboard::Key m_keyRotLeft = Keyboard::A;
Keyboard::Key m_keyRotRight = Keyboard::E;

Vector3f32 m_previousMousePos;
float32 m_mouseSensitivity = 0.1f;

private:
	Player* m_pMovement = nullptr;
	bool m_mouseLock = false;
	bool m_wasShooting = false;
public:

void Start() override
{
	m_pPlayer = m_pOwner;
	m_pMovement = m_pPlayer->GetScript<Player>();
}

void Update() override
{
	m_deltaTime = GameManager::DeltaTime();
	HandleInput();
	HandleMousePos();
}

void HandleInput()
{

	Vector3f32 direction = {0, 0, 0};
	
	if (GetKey(m_keyForward))
	 	direction += { 0, 0, 1 };
	if (GetKey(m_keyBackward))
	 	direction += { 0, 0, -1 };
	if (GetKey(m_keyLeft))
	 	direction += { -1, 0, 0 };
	if (GetKey(m_keyRight))
	 	direction += { 1, 0, 0 };

	if (GetKey(m_keyRotLeft))
		m_pPlayer->GetScript<Player>()->Die();

	Move(direction);

	if (GetKeyDown(Keyboard::P))
		m_pMovement->Test();
	
	if (GetKeyDown(m_keyJump))
		m_pPlayer->GetScript<Player>()->Jump();
	if (GetKey(m_keyJump))
		m_pPlayer->GetScript<Player>()->BoostUp();
	
	if (GetButtonDown(m_buttonLeft))
	{
		m_pPlayer->GetScript<Player>()->GetWeaponController()->BeginShot();
		m_wasShooting = true;
	}
		
	else if (GetButton(m_buttonLeft))
		m_pPlayer->GetScript<Player>()->GetWeaponController()->Shoot();
	else if (m_wasShooting == true)
	{
		m_pPlayer->GetScript<Player>()->GetWeaponController()->EndShot();
		m_wasShooting = false;
	}

	if (GetKeyDown(m_keyReload))
		m_pPlayer->GetScript<Player>()->GetWeaponController()->Reload();

	if (GetKeyDown(Keyboard::NUMPAD1))
		m_pPlayer->GetScript<Player>()->GetWeaponController()->EquipWeapon(0);

	if (GetKeyDown(Keyboard::NUMPAD2))
		m_pPlayer->GetScript<Player>()->GetWeaponController()->EquipWeapon(1);

	if (GetKeyDown(Keyboard::NUMPAD3))
		m_pPlayer->GetScript<Player>()->GetWeaponController()->EquipWeapon(2);

	if (GetKeyDown(m_keyEscape))
	{
		m_mouseLock = !m_mouseLock;
		if (m_mouseLock)
			HideMouseCursor();
		else
			ShowMouseCursor();
		Vector2i32 center( GameManager::GetWindow()->GetWidth() / 2, GameManager::GetWindow()->GetHeight() / 2 );
		SetMousePosition( { center.x, center.y } );
	}
}

void HandleMousePos()
{
	if (m_mouseLock == false)
		return;
	
	Vector2i32 center( GameManager::GetWindow()->GetWidth() / 2, GameManager::GetWindow()->GetHeight() / 2 );
	Vector2i32 mousePos = GetMousePosition();
	Vector2f32 delta = (mousePos - center);
	delta *= m_mouseSensitivity;

	m_pPlayer->transform.WorldRotate({0.0f, delta.x * m_deltaTime, 0.0f});
	m_pMovement->m_camera->GetOwner().transform.LocalRotate({delta.y * m_deltaTime, 0.0f, 0.0f});
	
	SetMousePosition( { center.x, center.y } );
}

void Move(Vector3f32 direction)
{
	m_pPlayer->GetScript<Player>()->Move(direction);
}

void Rotate(Vector3f32 rotation)
{
	m_pPlayer->GetScript<Player>()->Rotate(rotation);
}

END_SCRIPT

#endif // !PLAYERCONTROLLER_HPP_DEFINED
