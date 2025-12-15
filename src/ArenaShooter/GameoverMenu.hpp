#ifndef GAMEOVERMENU_HPP_INCLUDED
#define GAMEOVERMENU_HPP_INCLUDED

#include "define.h"
#include "Engine.h"
#include "SceneManager.h"
#include "CustomScene.h"
#include "GameoverController.hpp"

using namespace gce;

class GameoverMenu : public CustomScene
{


public:
    GameoverMenu(Scene* pScene) : CustomScene(pScene) {}

    void Init() override
    {
        CustomScene::Init();
        BitMapBrush* buttonStart = new BitMapBrush{ "res/ArenaShooter/recommencer_e.png" };
        BitMapBrush* buttonStart2 = new BitMapBrush{ "res/ArenaShooter/recommencer_a.png" };
        BitMapBrush* buttonMenu = new BitMapBrush{ "res/ArenaShooter/recommencer_e.png" };
        BitMapBrush* buttonMenu2 = new BitMapBrush{ "res/ArenaShooter/recommencer_a.png" };

        WindowParam* windowParam = &GameManager::GetWindowParam();

        GameObject& controller = AddObject();
        controller.AddScript<GameoverController>();

        GameObject& restartButton = AddObject();
        restartButton.transform.LocalTranslate({ 1400.f, 600.f, 0.0f });
        restartButton.transform.LocalScale({ 416.0f, 120.0f, 1.0f });
        UIButton& buttonRestart = *restartButton.AddComponent<UIButton>();

        GameObject& menuButton = AddObject();
        menuButton.transform.LocalTranslate({ 600.f, 600.f, 0.0f });
        menuButton.transform.LocalScale({ 416.0f, 120.0f, 1.0f });
        UIButton& buttonMenuM = *menuButton.AddComponent<UIButton>();

        buttonRestart.AddListener(&(controller.GetScript<GameoverController>()->GoToGame));
        buttonMenuM.AddListener(&(controller.GetScript<GameoverController>()->GoToMenu));

        buttonRestart.pBitMapBrush = buttonStart;
        buttonRestart.pHoverBitMapBrush = buttonStart2;

		buttonMenuM.pBitMapBrush = buttonMenu;
		buttonMenuM.pHoverBitMapBrush = buttonMenu2;
    }
};


#endif // !MAINMENU_HPP_INCLUDED
