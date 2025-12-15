#ifndef GAMEOVERCONTROLLER_HPP_INCLUDED
#define GAMEOVERCONTROLLER_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "GameManager.h"
#include "InputsMethods.h"
#include "SceneManager.h"

using namespace gce;


DECLARE_SCRIPT(GameoverController, ScriptFlag::Start | ScriptFlag::Update)

bool m_mouseLock = false;

void Start() override
{

}

void Update() override
{
	if (GetKeyDown(Keyboard::ESC))
	{
		m_mouseLock = !m_mouseLock;
		if (m_mouseLock)
			HideMouseCursor();
		else
			ShowMouseCursor();
		Vector2i32 center(GameManager::GetWindow()->GetWidth() / 2, GameManager::GetWindow()->GetHeight() / 2);
		SetMousePosition({ center.x, center.y });
	}
}

static void GoToGame()
{
	SceneManager::GetInstance()->ChangeScene(GAME);
}

static void GoToMenu()
{
	SceneManager::GetInstance()->ChangeScene(MAIN_MENU);
}

static void Quit()
{

}

END_SCRIPT

#endif