#ifndef CONTROLLER_H_INCLUDED
#define CONTROLLER_H_INCLUDED

#include "define.h"
#include "InputsMethods.h"
#include "Player.hpp"

namespace gce {


	class Controller
	{
	public:
		void Init(GameObject* player);
		void Update(float32 deltaTime);
		void HandleInput(float32 deltaTime);

		bool IsUp() const { return m_up; }
		bool IsDown() const { return m_down; }
		bool IsLeft() const { return m_left; }
		bool IsRight() const { return m_right; }

		void SetUp(bool up) { m_up = up; }
		void SetDown(bool down) { m_down = down; }
		void SetLeft(bool left) { m_left = left; }
		void SetRight(bool right) { m_right = right; }

		void Move(float32 deltaTime, Vector3f32 direction);

	private:
		GameObject* mp_player;

		Keyboard::Key m_keyUp = Keyboard::Z;
		Keyboard::Key m_keyDown = Keyboard::S;
		Keyboard::Key m_keyLeft = Keyboard::Q;
		Keyboard::Key m_keyRight = Keyboard::D;

		bool m_up;
		bool m_down;
		bool m_left;
		bool m_right;

	};


}

#endif


