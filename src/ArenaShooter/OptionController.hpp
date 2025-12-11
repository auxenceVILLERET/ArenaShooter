#ifndef OPTIONSCONTROLLER_INCLUDED
#define OPTIONSCONTROLLER_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "GameManager.h"
#include "InputsMethods.h"
#include "SceneManager.h"

using namespace gce;


DECLARE_SCRIPT(OptionController, ScriptFlag::Start | ScriptFlag::Update)



void Start() override
{

}

void Update() override
{

}

static void GoToMainMenu()
{
	SceneManager::GetInstance()->ChangeScene(MAIN_MENU);
}



END_SCRIPT

#endif