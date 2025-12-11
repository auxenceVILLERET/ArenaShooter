#ifndef OPTIONSMENU_HPP_INCLUDED
#define OPTIONSMENU_HPP_INCLUDED

#include "define.h"
#include "CustomScene.h"
#include <Engine.h>
#include "OptionController.hpp"

using namespace gce;

void InitOptionsMenu(CustomScene* menu, WindowParam* windowParam)
{
    BitMapBrush* buttonBrush = new BitMapBrush{ "res/ArenaShooter/VilleretAuxence.jpg" };
    BitMapBrush* button2Brush = new BitMapBrush{ "res/ArenaShooter/VilleretAuxence.jpg" };

    GameObject& menu_controller = menu->AddObject();
    menu_controller.AddScript<OptionController>();

    GameObject& playButton = menu->AddObject();
    playButton.transform.LocalTranslate({ windowParam->width / 2.f, 200.f, 0.0f });
    playButton.transform.LocalScale({ 216.0f, 69.0f, 1.0f });
    UIButton& button = *playButton.AddComponent<UIButton>();


    button.AddListener(&(menu_controller.GetScript<OptionController>()->GoToMainMenu));

    button.pBitMapBrush = buttonBrush;
    button.pHoverBitMapBrush = button2Brush;
}

#endif // !1
