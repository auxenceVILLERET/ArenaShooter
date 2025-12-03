#include "Controller.h"

namespace gce
{
	void Controller::Init()
	{
		m_up = m_down = m_left = m_right = false;
	}

	void Controller::Update(float32 deltaTime)
	{
		HandleInput(deltaTime);
	}

	void Controller::HandleInput(float32 deltaTime)
	{
		if(GetKeyDown(m_keyUp))
			MoveForward(deltaTime);
		if (GetKeyDown(m_keyDown))
			MoveBackward(deltaTime);
		if (GetKeyDown(m_keyLeft))
			MoveLeft(deltaTime);
		if (GetKeyDown(m_keyRight))
			MoveRight(deltaTime);

	}
}