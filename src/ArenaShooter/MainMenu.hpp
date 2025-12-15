#ifndef MAINMENU_HPP_INCLUDED
#define MAINMENU_HPP_INCLUDED

#include "define.h"
#include "Engine.h"
#include "SceneManager.h"
#include "CustomScene.h"
#include "MenuController.hpp"

using namespace gce;


void InitMenu(CustomScene* menu, WindowParam* windowParam)
{
    BitMapBrush* buttonStart = new BitMapBrush{ "res/ArenaShooter/commencer_e.png"};
    BitMapBrush* buttonStart2 = new BitMapBrush{ "res/ArenaShooter/commencer_allumé.png" };
    BitMapBrush* buttonOptionsBrush = new BitMapBrush{ "res/ArenaShooter/OPTIONS.PNG" };
    BitMapBrush* buttonQuitBrush = new BitMapBrush{ "res/ArenaShooter/QUIT.PNG" };
    BitMapBrush* button2Brush = new BitMapBrush{ "res/ArenaShooter/VilleretAuxence.jpg" };

    GameObject& menu_controller = menu->AddObject();
    menu_controller.AddScript<MenuController>();

    GameObject& title = menu->AddObject();
    ImageUI& uiImage = *title.AddComponent<ImageUI>();
    Vector2f32 center = { windowParam->width / 2.f, windowParam->height / 2.f };
    Vector2f32 size = { 32, 32 };
    Vector2f32 posUi = center - size * 0.5f;
    uiImage.InitializeImage(posUi, size, 1.f);

    uiImage.btmBrush = new BitMapBrush("res/ArenaShooter/VilleretAuxence.jpg");
    uiImage.btmBrush->SetTransformMatrix({ posUi.x, posUi.y, 0 }, { 1.f / 16.f, 1.f / 16.f, 1.f / 16.f }, 0);
    uiImage.SetActive(true);

    GameObject& playButton = menu->AddObject();
    playButton.transform.LocalTranslate({ 1000.f, 400.f, 0.0f });
    playButton.transform.LocalScale({ 416.0f, 120.0f, 1.0f });
    UIButton& button = *playButton.AddComponent<UIButton>();

    GameObject& optionButton = menu->AddObject();
    optionButton.transform.LocalTranslate({ 1000.f, 600.f, 0.0f });
    optionButton.transform.LocalScale({ 416.0f, 120.0f, 1.0f });
    UIButton& button2 = *optionButton.AddComponent<UIButton>();

    GameObject& quitButton = menu->AddObject();
    quitButton.transform.LocalTranslate({ 1000.f, 800.f, 0.0f });
    quitButton.transform.LocalScale({ 416.0f, 120.0f, 1.0f });
    UIButton& button3 = *quitButton.AddComponent<UIButton>();

    button.AddListener(&(menu_controller.GetScript<MenuController>()->GoToGame));
    button2.AddListener(&(menu_controller.GetScript<MenuController>()->GoToOption));

    button.pBitMapBrush = buttonStart;
    button.pHoverBitMapBrush = buttonStart2;

    button2.pBitMapBrush = buttonOptionsBrush;
    button2.pHoverBitMapBrush = button2Brush;

    button3.pBitMapBrush = buttonQuitBrush;
    button3.pHoverBitMapBrush = button2Brush;
}



#endif // !MAINMENU_HPP_INCLUDED
