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
        Vector2f32 size = { 1920, 1080 };
        uiImage.InitializeImage({0,0}, size, 1.f);

        uiImage.btmBrush = new BitMapBrush("res/ArenaShooter/Ecran_titre.png");
        uiImage.btmBrush->SetTransformMatrix({ 0, 0, 0 }, { 1.f , 1.f , 1.f }, 0);
        uiImage.SetActive(true);

        GameObject& playButton = AddObject();
        playButton.transform.LocalTranslate({ 1000.f, 650.f, 0.0f });
        playButton.transform.LocalScale({ 564.0f, 120.0f, 1.0f });
        UIButton& button = *playButton.AddComponent<UIButton>();

        GameObject& optionButton = AddObject();
        optionButton.transform.LocalTranslate({ 1000.f, 800.f, 0.0f });
        optionButton.transform.LocalScale({ 552.0f, 120.0f, 1.0f });
        UIButton& button2 = *optionButton.AddComponent<UIButton>();

        GameObject& quitButton = AddObject();
        quitButton.transform.LocalTranslate({ 1000.f, 950.f, 0.0f });
        quitButton.transform.LocalScale({ 396.0f, 120.0f, 1.0f });
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
