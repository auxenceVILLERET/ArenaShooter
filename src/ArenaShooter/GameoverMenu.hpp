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
        BitMapBrush* buttonMenu = new BitMapBrush{ "res/ArenaShooter/retour_au_menu_e.png" };
        BitMapBrush* buttonMenu2 = new BitMapBrush{ "res/ArenaShooter/retour_au_menu_a.png" };

        WindowParam* windowParam = &GameManager::GetWindowParam();

        GameObject& controller = AddObject();
        controller.AddScript<GameoverController>();

        GameObject& Background = AddObject();
        ImageUI& uiImage = *Background.AddComponent<ImageUI>();
        Vector2f32 size = { 1920, 1080 };
        uiImage.InitializeImage({ 0,0 }, size, 1.f);
        uiImage.btmBrush = new BitMapBrush("res/ArenaShooter/GameOverFond.PNG");
        uiImage.btmBrush->SetTransformMatrix({ 0, 0, 0 }, { 1.f , 1.f , 1.f }, 0);
        uiImage.SetActive(true);

        GameObject& GameOverTitle = AddObject();
        ImageUI& uiImage2 = *GameOverTitle.AddComponent<ImageUI>();
        uiImage2.InitializeImage({ 0,0 }, size, 1.f);
        uiImage2.btmBrush = new BitMapBrush("res/ArenaShooter/GameOver1.PNG");
        uiImage2.btmBrush->SetTransformMatrix({ 0, 0, 0 }, { 1.f , 1.f , 1.f }, 0);
        uiImage2.SetActive(true);

        GameObject& Bar = AddObject();
        ImageUI& uiImage3 = *Bar.AddComponent<ImageUI>();
        uiImage3.InitializeImage({ 0,0 }, size, 1.f);
        uiImage3.btmBrush = new BitMapBrush("res/ArenaShooter/GameOver2.PNG");
        uiImage3.btmBrush->SetTransformMatrix({ 0, 0, 0 }, { 1.f , 1.f , 1.f }, 0);
        uiImage3.SetActive(true);

        GameObject& restartButton = AddObject();
        restartButton.transform.LocalTranslate({ 1450.f, 800.f, 0.0f });
        restartButton.transform.LocalScale({ 648.f, 120.0f, 1.0f });
        UIButton& buttonRestart = *restartButton.AddComponent<UIButton>();

        GameObject& menuButton = AddObject();
        menuButton.transform.LocalTranslate({ 500.f, 800.f, 0.0f });
        menuButton.transform.LocalScale({ 708.f, 120.0f, 1.0f });
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
