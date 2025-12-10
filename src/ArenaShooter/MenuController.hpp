#ifndef MENUCONTROLLER_HPP_INCLUDED
#define MENUCONTROLLER_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "GameManager.h"
#include "InputsMethods.h"
#include "SceneManager.h"

using namespace gce;


DECLARE_SCRIPT(MenuController, ScriptFlag::Start | ScriptFlag::Update)



void Start() override
{

}

void Update() override
{
	HandleInput();
}

void HandleInput()
{
	if (GetKeyDown(Keyboard::ENTER))
	{
		SceneManager::GetInstance()->ChangeScene(GAME);
	}
}

END_SCRIPT

#endif