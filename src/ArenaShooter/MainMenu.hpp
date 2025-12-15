#ifndef MAINMENU_HPP_INCLUDED
#define MAINMENU_HPP_INCLUDED

#include "define.h"
#include "Engine.h"
#include "SceneManager.h"
#include "CustomScene.h"
#include "MenuController.hpp"

using namespace gce;

class MainMenu : public CustomScene
{
   

public:
    MainMenu(Scene* pScene) : CustomScene(pScene) {}
    
    void Init() override
    {
        CustomScene::Init();
        BitMapBrush* buttonStart = new BitMapBrush{ "res/ArenaShooter/commencer_e.png" };
        BitMapBrush* buttonStart2 = new BitMapBrush{ "res/ArenaShooter/commencer_a.png" };
        BitMapBrush* buttonOptionsBrush = new BitMapBrush{ "res/ArenaShooter/parametres_e.png" };
        BitMapBrush* buttonOptionsBrush2 = new BitMapBrush{ "res/ArenaShooter/parametres_a.png" };
        BitMapBrush* buttonQuitBrush = new BitMapBrush{ "res/ArenaShooter/quitter_e.png" };
        BitMapBrush* buttonQuitBrush2 = new BitMapBrush{ "res/ArenaShooter/quitter_a.png" };
        
        GameObject& menu_controller = AddObject();
        menu_controller.AddScript<MenuController>();

        WindowParam* windowParam = &GameManager::GetWindowParam();
        
        GameObject& title = AddObject();
        ImageUI& uiImage = *title.AddComponent<ImageUI>();
        Vector2f32 center = { windowParam->width / 2.f, windowParam->height / 2.f };
        Vector2f32 size = { 32, 32 };
        Vector2f32 posUi = center - size * 0.5f;
        uiImage.InitializeImage(posUi, size, 1.f);

        uiImage.btmBrush = new BitMapBrush("res/ArenaShooter/VilleretAuxence.jpg");
        uiImage.btmBrush->SetTransformMatrix({ posUi.x, posUi.y, 0 }, { 1.f / 16.f, 1.f / 16.f, 1.f / 16.f }, 0);
        uiImage.SetActive(true);

        GameObject& playButton = AddObject();
        playButton.transform.LocalTranslate({ 1000.f, 400.f, 0.0f });
        playButton.transform.LocalScale({ 416.0f, 120.0f, 1.0f });
        UIButton& button = *playButton.AddComponent<UIButton>();

        GameObject& optionButton = AddObject();
        optionButton.transform.LocalTranslate({ 1000.f, 600.f, 0.0f });
        optionButton.transform.LocalScale({ 416.0f, 120.0f, 1.0f });
        UIButton& button2 = *optionButton.AddComponent<UIButton>();

        GameObject& quitButton = AddObject();
        quitButton.transform.LocalTranslate({ 1000.f, 800.f, 0.0f });
        quitButton.transform.LocalScale({ 416.0f, 120.0f, 1.0f });
        UIButton& button3 = *quitButton.AddComponent<UIButton>();

        button.AddListener(&(menu_controller.GetScript<MenuController>()->GoToGame));
        button2.AddListener(&(menu_controller.GetScript<MenuController>()->GoToOption));
		button3.AddListener(&(menu_controller.GetScript<MenuController>()->Quit));

        button.pBitMapBrush = buttonStart;
        button.pHoverBitMapBrush = buttonStart2;

        button2.pBitMapBrush = buttonOptionsBrush;
        button2.pHoverBitMapBrush = buttonOptionsBrush2;

        button3.pBitMapBrush = buttonQuitBrush;
        button3.pHoverBitMapBrush = buttonQuitBrush2;
    }
};


#endif // !MAINMENU_HPP_INCLUDED
