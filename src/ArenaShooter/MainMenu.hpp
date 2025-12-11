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
    BitMapBrush* buttonBrush = new BitMapBrush{ "res/ArenaShooter/VilleretAuxence.jpg"};
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
    playButton.transform.LocalTranslate({ windowParam->width / 2.f, 200.f, 0.0f });
    playButton.transform.LocalScale({ 216.0f, 69.0f, 1.0f });
    UIButton& button = *playButton.AddComponent<UIButton>();

    GameObject& optionButton = menu->AddObject();
    optionButton.transform.LocalTranslate({ windowParam->width / 2.f, 500.f, 0.0f });
    optionButton.transform.LocalScale({ 216.0f, 69.0f, 1.0f });
    UIButton& button2 = *optionButton.AddComponent<UIButton>();

    button.AddListener(&(menu_controller.GetScript<MenuController>()->GoToGame));
    button2.AddListener(&(menu_controller.GetScript<MenuController>()->GoToOption));

    button.pBitMapBrush = buttonBrush;
    button.pHoverBitMapBrush = button2Brush;
    button2.pBitMapBrush = buttonBrush;
    button2.pHoverBitMapBrush = button2Brush;
}



#endif // !MAINMENU_HPP_INCLUDED
