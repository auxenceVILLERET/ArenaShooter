#include "Controller.h"

namespace gce
{
	void Controller::Init(GameObject* player)
	{
		mp_player = player;

		m_up = m_down = m_left = m_right = false;
	}

	void Controller::Update(float32 deltaTime)
	{
		HandleInput(deltaTime);
	}

	void Controller::HandleInput(float32 deltaTime)
	{
		if(GetKeyDown(m_keyUp))
			Move(deltaTime, { 0, 0, 1 });
		if (GetKeyDown(m_keyDown))
			Move(deltaTime, { 0, 0, -1 });
		if (GetKeyDown(m_keyLeft))
			Move(deltaTime, { -1, 0, 0 });
		if (GetKeyDown(m_keyRight))
			Move(deltaTime, { 1, 0, 0 });

	}

	void Controller::Move(float32 deltaTime, Vector3f32 direction)
	{
		mp_player->GetScript<PlayerMovement>()->Move(direction);
	}
}